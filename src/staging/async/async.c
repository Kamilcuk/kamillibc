/*
 * async.c
 *
 *  Created on: 17 lip 2018
 *      Author: Kamil Cukrowski
 *     License: Jointly under MIT License and Beerware License
 */
#include "async.h"

#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>

#include <errno.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __GNUC__
#define __builtin_expect(expr, val)  ((expr) == (val))
#endif

#define try(expr)  do{ if (__builtin_expect(!(expr), 0)) { async_fail_handler(__FILE__, __LINE__, __func__, #expr); } }while(0)

#define dbg(str, ...)  ((void)0)
//#define dbg(str, ...)  fprintf(stderr, str, ##__VA_ARGS__)

#define dbgln(str, ...)  dbg("%15ld:%5d:%-21s : " str "\n", \
    ((unsigned long)pthread_self()), __LINE__, __func__, ##__VA_ARGS__)

/* Private Functions ------------------------------------------------------- */

static inline async_t *async_alloc(void)
{
	async_t *t = malloc(sizeof(*t));
	if (t == NULL) {
		goto MALLOC_FAIL;
	}

	memset(t, 0, sizeof(*t));
	if ((t->readyfd = eventfd(0, 0)) < 0) {
		goto EVENTFD_FAIL;
	}
	if (pthread_mutex_init(&t->mutex, NULL)) {
		goto PTHREAD_MUTEX_INIT_MUTEX_FAIL;
	}
	dbgln("%p", (void*)t);
	return t;

	try(pthread_mutex_destroy(&t->mutex) == 0);
	PTHREAD_MUTEX_INIT_MUTEX_FAIL:
	try(close(t->readyfd) == 0);
	EVENTFD_FAIL:
	free(t);
	MALLOC_FAIL:
	return NULL;
}

static void async_free(async_t *t)
{
	if (t == NULL)
		return;
	try(pthread_mutex_destroy(&t->mutex) == 0);
	try(close(t->readyfd) == 0);
	free(t);
}

static int async_poll_ignore_signal(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int ret;
    while ((ret = poll(fds, nfds, timeout)) == -1 && errno == EINTR);
    try(0 <= ret && (nfds_t)ret <= nfds);
    return ret;
}

static inline void async_notify_ready(async_t *t)
{
    try(write(t->readyfd, (uint64_t[1]){1}, sizeof(uint64_t)) == sizeof(uint64_t));
}

static void async_thread_cleanup(void *arg0)
{
	async_t *t = arg0;
	try(pthread_mutex_lock(&t->mutex) == 0);
	if (t->do_free) {
		dbgln("%p freeing myself", (void*)t);
		try(pthread_mutex_unlock(&t->mutex) == 0);
		async_free(t);
		try(pthread_detach(pthread_self()) == 0);
		return;
	} else {
		dbgln("%p no free myself, only readying", (void*)t);
		async_notify_ready(t);
		try(pthread_mutex_unlock(&t->mutex) == 0);
	}
}

static void *async_thread(void *arg0) {
	async_t *t = arg0;
	assert(t != NULL);
	pthread_cleanup_push(async_thread_cleanup, t);

	dbgln("%p Running %p(%p)", (void*)t, (void*)t->f, t->arg);
	t->returned = t->f_voidptr(t->arg);
	dbgln("%p Running %p(%p) returned %p", (void*)t, (void*)t->f, t->arg, t->returned);

	pthread_cleanup_pop(1);
	return NULL;
}

static void *async_thread_when_all(void *arg0)
{
	async_t *t = arg0;
	assert(t != NULL);
	pthread_cleanup_push(async_thread_cleanup, t);

	dbgln("%p asyncs_wait(%p, %zu)", (void*)t, (void*)t->asyncs, t->asyncs_cnt);
	asyncs_wait(t->asyncs, t->asyncs_cnt);

	dbgln("%p Running %p(%p, %zu)", (void*)t, (void*)t->f, (void*)t->asyncs, t->asyncs_cnt);
	t->returned = t->f_asyncarr(t->asyncs, t->asyncs_cnt);
	dbgln("%p Running %p(%p, %zu) returned %p", (void*)t, (void*)t->f, (void*)t->asyncs, t->asyncs_cnt, t->returned);

	pthread_cleanup_pop(1);
	return NULL;
}

static void *async_thread_when_any(void *arg0)
{
	async_t *t = arg0;
	assert(t != NULL);
	pthread_cleanup_push(async_thread_cleanup, t);

    try(asyncs_anywait_for(t->asyncs, t->asyncs_cnt, -1) >= 0);

	dbgln("%p Running %p(%p,%zu)", (void*)t, (void*)t->f, (void*)t->asyncs, t->asyncs_cnt);
	t->returned = t->f_asyncarr(t->asyncs, t->asyncs_cnt);
	dbgln("%p Running %p(%p,%zu) returned %p", (void*)t, (void*)t->f, (void*)t->asyncs, t->asyncs_cnt, t->returned);

	pthread_cleanup_pop(1);
	return NULL;
}


/* Exported Functions ------------------------------------------------------------- */

async_t *async_create(void *(*f)(void*), void *arg)
{
	assert(f != NULL);

	async_t *t = async_alloc();
	if (t == NULL) {
		goto ASYNC_ALLOC_FAIL;
	}

	t->f_voidptr = f;
	t->arg = arg;

	if (pthread_create(&t->thread, NULL, async_thread, t)) {
		goto PTHREAD_CREATE_FAIL;
	}

	return t;

	pthread_cancel(t->thread);
	PTHREAD_CREATE_FAIL:
	async_free(t);
	ASYNC_ALLOC_FAIL:
	return NULL;
}

async_t *async_create_arg_self(void *(*f)(async_t*))
{
	assert(f != NULL);

	async_t *t = async_alloc();
	if (t == NULL) {
		goto ASYNC_ALLOC_FAIL;
	}

	t->f_voidptr = (void *(*)(void*))f;
	t->arg = t;

	if (pthread_create(&t->thread, NULL, async_thread, t)) {
		goto PTHREAD_CREATE_FAIL;
	}

	return t;

	pthread_cancel(t->thread);
	PTHREAD_CREATE_FAIL:
	async_free(t);
	ASYNC_ALLOC_FAIL:
	return NULL;
}

async_t *async_then(async_t *t, void *(*f)(async_t*))
{
	return async_create((void*(*)(void*))f, (void*)t);
}

async_t *async_when_all(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt))
{
	assert(f != NULL);

	async_t *t = async_alloc();
	if (t == NULL) {
		goto ASYNC_ALLOC_FAIL;
	}

	t->f_asyncarr = f;
	t->asyncs = asyncs;
	t->asyncs_cnt = asyncs_cnt;

	if (pthread_create(&t->thread, NULL, async_thread_when_all, t)) {
		goto PTHREAD_CREATE_FAIL;
	}

	return t;

	pthread_cancel(t->thread);
	PTHREAD_CREATE_FAIL:
	async_free(t);
	ASYNC_ALLOC_FAIL:
	return NULL;
}

async_t *async_when_any(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt))
{
	assert(f != NULL);

	async_t *t = async_alloc();
	if (t == NULL) {
		goto ASYNC_ALLOC_FAIL;
	}

	t->f_asyncarr = f;
	t->asyncs = asyncs;
	t->asyncs_cnt = asyncs_cnt;

	if (pthread_create(&t->thread, NULL, async_thread_when_any, t)) {
		goto PTHREAD_CREATE_FAIL;
	}

	return t;

	try(pthread_cancel(t->thread) == 0);
	PTHREAD_CREATE_FAIL:
	async_free(t);
	ASYNC_ALLOC_FAIL:
	return NULL;
}

/* Destructors -------------------------------------------------------- */

void async_detach(async_t **t0)
{
	assert(t0 != NULL);
	async_t *t = *t0;
	assert(t != NULL);
    try(pthread_mutex_lock(&t->mutex) == 0);
	try(pthread_detach(t->thread) == 0);
	if (async_is_ready(t)) {
		dbgln("%p %ld is ready, freeing", (void*)t, t->thread);
	    try(pthread_mutex_unlock(&t->mutex) == 0);
		async_free(t);
	} else {
		dbgln("%p %ld not ready, detaching", (void*)t, t->thread);
		t->do_free = true;
		try(pthread_mutex_unlock(&t->mutex) == 0);
	}
	*t0 = NULL;
}

void async_cancel(async_t **t0)
{
	assert(t0 != NULL);
	async_t *t = *t0;
	assert(t != NULL);
    try(pthread_mutex_lock(&t->mutex) == 0);
	if (async_is_ready(t)) {
		dbgln("%p %ld is ready, freeing", (void*)t, t->thread);
		try(pthread_detach(t->thread) == 0);
	    try(pthread_mutex_unlock(&t->mutex) == 0);
		async_free(t);
	} else {
	    dbgln("%p %ld not ready, canceling", (void*)t, t->thread);
	    try(pthread_cancel(t->thread) == 0);
	    t->do_free = true;
	    try(pthread_mutex_unlock(&t->mutex) == 0);
	}
	*t0 = NULL;
}

/* Methods ------------------------------------------------------------------- */

bool async_is_ready(async_t *t) {
	assert(t != NULL);
	const bool ret = async_wait_for(t, 0);
	dbgln("%p ret=%d", (void*)t, ret);
	return ret;
}

void *async_get(async_t *t) {
	assert(t != NULL);
	dbgln("%p thread=%ld", (void*)t, t->thread);
	try(async_wait_for(t, -1) == true);
	try(pthread_mutex_lock(&t->mutex) == 0);
	void * const ret = t->returned;
	try(pthread_mutex_unlock(&t->mutex) == 0);
	dbgln("%p thread=%ld returned %p", (void*)t, t->thread, ret);
	return ret;
}

void async_wait(async_t *t)
{
	try(async_wait_for(t, -1) == true);
}

bool async_wait_for(async_t *t, int timeout)
{
    struct pollfd fds[1];
    fds[0].events = POLLIN;
    fds[0].fd = t->readyfd;
    return async_poll_ignore_signal(fds, 1, timeout) == 1;
}

/* Asyncs -------------------------------------------------------------- */

void asyncs_detach(async_t *asyncs[], size_t asyncs_cnt)
{
	for (size_t i = 0 ; i < asyncs_cnt; ++i) {
		async_detach(&asyncs[i]);
	}
}

void asyncs_cancel(async_t *asyncs[], size_t asyncs_cnt)
{
	for (size_t i = 0 ; i < asyncs_cnt; ++i) {
		async_cancel(&asyncs[i]);
	}
}

void asyncs_wait(async_t *asyncs[], size_t asyncs_cnt)
{
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		async_wait(asyncs[i]);
	}
}

int asyncs_wait_for(async_t *asyncs[], size_t asyncs_cnt, int timeout)
{
	// TODO:
	(void)asyncs;
	(void)asyncs_cnt;
	(void)timeout;
	assert(0);
	return -1;
}

int asyncs_anywait_for(async_t *asyncs[], size_t asyncs_cnt, int timeout)
{
	struct pollfd *fds = calloc(asyncs_cnt, sizeof(*fds));
	if (fds == NULL) {
		return -1;
	}
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		fds[i].events = POLLIN;
		fds[i].fd = asyncs[i]->readyfd;
	}
	const int ret = async_poll_ignore_signal(fds, asyncs_cnt, timeout);
	free(fds);
	return ret;
}

/* Weak Functions ---------------------------------------------------------- */

__attribute__((__weak__))
void async_fail_handler(const char file[], int line, const char func[], const char expr[])
{
	fprintf(stderr,
			"%s:%d: Error: %s(): Expression '%s' failed\n",
			file, line, func, expr);
	abort();
}

