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
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ------------------------------------------------------------------------- */

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#else
#define __builtin_expect(expr, val)  ((expr) == (val))
#endif

#define try(expr)  do{ \
	if (__builtin_expect(!(expr), 0)) { \
		async_fail_handler(__FILE__, __LINE__, __func__, #expr); \
		abort(); \
	} \
}while(0)
#define tryret(expr, ...) do{ \
	if (__builtin_expect(!(expr), 0)) { \
		async_fail_handler(__FILE__, __LINE__, __func__, #expr); \
		return __VA_ARGS__; \
	} \
}while(0)

#define dbgln(str, ...)  ((void)0)
#ifndef dbgln
#include <execinfo.h>
static pthread_mutex_t sout_mutex = PTHREAD_MUTEX_INITIALIZER;
static inline void _dbgln(int line, const char func[], const char fmt[], ...)
{
	FILE * const sout = stderr;

	va_list va;
	va_start(va, fmt);

	struct timespec t;
	try(clock_gettime(CLOCK_REALTIME, &t) == 0);

	try(pthread_mutex_lock(&sout_mutex) == 0);

	fprintf(sout, "%2ld.%06ld: %15ld:%5d:%-21s : ", t.tv_sec%100, t.tv_nsec, (unsigned long)pthread_self(), line, func);

	vfprintf(sout, fmt, va);

#if 0
	void *backbuf[10];
	int backbuflen = backtrace(backbuf, sizeof(backbuf)/sizeof(backbuf[0]));
	try(backbuflen > 1);
	char **backsym = backtrace_symbols(backbuf, backbuflen);
	try(backsym != NULL);
	fprintf(sout, " ");
	for (int i = 0; i < backbuflen - 1; ++i) {
		assert(backsym[i] != NULL);
		fprintf(sout, "%s->", backsym[i]);
	}
	free(backsym);
#endif

	fprintf(sout, "\n");

	try(pthread_mutex_unlock(&sout_mutex) == 0);
	va_end(va);
}
#define dbgln(str, ...)   _dbgln(__LINE__, __func__, str,  ##__VA_ARGS__)
#endif

/* Private Functions Forward Declarations ---------------------------------- */

static void async_free(async_t *t);
static void async_refref_up(async_t *t);
static void async_refref_down(async_t *t);

/* Private Functions ------------------------------------------------------- */

static inline int async_mutex_lock(async_t *t)
{
	assert(t != NULL);
	dbgln("%p %u", (void*)t, t->mutex.__data.__nusers);
	return pthread_mutex_lock(&t->mutex);
}

static inline int async_mutex_unlock(async_t *t)
{
	assert(t != NULL);
	dbgln("%p %u", (void*)t, t->mutex.__data.__nusers);
	return pthread_mutex_unlock(&t->mutex);
}

static int async_poll_ignore_signal(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int ret;
    while ((ret = poll(fds, nfds, timeout)) == -1 && errno == EINTR);
    assert ((nfds_t)ret <= nfds);
    return ret;
}

static inline void async_notify_ready(async_t *t)
{
    tryret(write(t->readyfd, (uint64_t[1]){1}, sizeof(uint64_t)) == sizeof(uint64_t));
}

static void async_ref_up_in(async_t *t)
{
	assert(t != NULL);
	assert(pthread_mutex_trylock(&t->mutex) == EBUSY);
	assert(t->refcnt < UINT_MAX);
	++t->refcnt;
	dbgln("%p ref=%d", (void*)t, t->refcnt);
}

static void async_ref_up(async_t *t)
{
	assert(t != NULL);
	tryret(async_mutex_lock(t) == 0);
	async_ref_up_in(t);
	tryret(async_mutex_unlock(t) == 0);
}

static bool async_ref_down_in(async_t *t)
{
	assert(t != NULL);
	assert(pthread_mutex_trylock(&t->mutex) == EBUSY);
	assert(t->refcnt > 0);
	--t->refcnt;
	if (t->refcnt == 0) {
		dbgln("%p ref=%u freeing", (void*)t, t->refcnt);
		try(pthread_detach(t->thread) == 0);
		try(async_mutex_unlock(t) == 0);
		async_free(t);
		return true;
	} else {
		dbgln("%p ref=%u", (void*)t, t->refcnt);
	}
	return false;
}

static void async_ref_down(async_t *t)
{
	assert(t != NULL);
	dbgln("%p", (void*)t);
	try(async_mutex_lock(t) == 0);
	if (!async_ref_down_in(t)) {
		try(async_mutex_unlock(t) == 0);
	}
}

static void async_thread_cleanup(void *arg0)
{
	async_t *t = arg0;
	assert(t != NULL);
	try(async_mutex_lock(t) == 0);
	if (!async_ref_down_in(t)) {
		dbgln("%p not freed, readying", (void*)t);
		async_notify_ready(t);
		try(async_mutex_unlock(t) == 0);
	}
}

static void async_task_execute(async_t *t)
{
	assert(t != NULL);
	assert(0 <= t->type && t->type <= (int)ASYNC_TYPE_MAX);
	switch(t->type) {
	case ASYNC_TYPE_VOID:
		dbgln("%p Running %p(%p)", (void*)t, (void*)(uintptr_t)t->c.v.f, t->c.v.arg);
		assert(t->c.v.f != NULL);
		t->returned = t->c.v.f(t->c.v.arg);
		dbgln("%p Running %p(%p) returned %p", (void*)t, (void*)(uintptr_t)t->c.v.f, t->c.v.arg, t->returned);
		break;
	case ASYNC_TYPE_THEN:
		dbgln("%p Running %p(%p)", (void*)t, (void*)(uintptr_t)t->c.t.f, (void*)t->c.t.arg);
		assert(t->c.t.f != NULL);
		t->returned = t->c.t.f(t->c.t.arg);
		dbgln("%p Running %p(%p) returned %p", (void*)t, (void*)(uintptr_t)t->c.t.f, (void*)t->c.t.arg, t->returned);
		break;
	case ASYNC_TYPE_WHENALL:
	case ASYNC_TYPE_WHENANY:
		switch(t->type) {
		case ASYNC_TYPE_WHENALL:
			dbgln("%p asyncs_wait(%p, %zu)", (void*)t, (void*)t->c.w.asyncs, t->c.w.asyncs_cnt);
			try(asyncs_wait(t->c.w.asyncs, t->c.w.asyncs_cnt) == 0);
			break;
		case ASYNC_TYPE_WHENANY:
			dbgln("%p asyncs_anywait_for(%p, %zu, -1)", (void*)t, (void*)t->c.w.asyncs, t->c.w.asyncs_cnt);
		    try(asyncs_anywait_for(t->c.w.asyncs, t->c.w.asyncs_cnt, -1) >= 0);
		    break;
		default:
			break;
		}
		dbgln("%p Running %p(%p,%zu)", (void*)t, (void*)(uintptr_t)t->c.w.f, (void*)t->c.w.asyncs, t->c.w.asyncs_cnt);
		assert(t->c.w.f != NULL);
		t->returned = t->c.w.f(t->c.w.asyncs, t->c.w.asyncs_cnt);
		dbgln("%p Running %p(%p,%zu) returned %p", (void*)t, (void*)(uintptr_t)t->c.w.f, (void*)t->c.w.asyncs, t->c.w.asyncs_cnt, t->returned);
		break;
	}
}

static void *async_thread(void *arg0)
{
	async_t *t = arg0;
	assert(t != NULL);
	pthread_cleanup_push(async_thread_cleanup, t);
	dbgln("%p startup", (void*)t);
	try(async_mutex_unlock(t) == 0);

	async_task_execute(t);

	pthread_cleanup_pop(1);
	return NULL;
}

static void async_refref_up(async_t *t)
{
	assert(t != NULL);
	switch(t->type) {
	case ASYNC_TYPE_VOID:
		break;
	case ASYNC_TYPE_THEN:
		dbgln("%p -> %p", (void*)t, (void*)t->c.t.arg);
		assert(t->c.t.arg != NULL);
		async_ref_up(t->c.t.arg);
		break;
	case ASYNC_TYPE_WHENALL:
	case ASYNC_TYPE_WHENANY:
		for (size_t i = 0; i < t->c.w.asyncs_cnt; ++i) {
			dbgln("%p -> %p", (void*)t, (void*)t->c.w.asyncs[i]);
			assert(t->c.w.asyncs[i] != NULL);
			async_ref_up(t->c.w.asyncs[i]);
		}
	}
}

static void async_refref_down(async_t *t)
{
	assert(t != NULL);
	switch(t->type) {
	case ASYNC_TYPE_VOID:
		break;
	case ASYNC_TYPE_THEN:
		dbgln("%p -> %p", (void*)t, (void*)t->c.t.arg);
		assert(t->c.t.arg != NULL);
		async_ref_down(t->c.t.arg);
		break;
	case ASYNC_TYPE_WHENALL:
	case ASYNC_TYPE_WHENANY:
		for (size_t i = 0; i < t->c.w.asyncs_cnt; ++i) {
			dbgln("%p -> %p", (void*)t, (void*)t->c.w.asyncs[i]);
			assert(t->c.w.asyncs[i] != NULL);
			async_ref_down(t->c.w.asyncs[i]);
		}
	}
}

static int async_start_pthread(async_t *t)
{
	async_ref_up(t);

	try(async_mutex_lock(t) == 0);

	if (pthread_create(&t->thread, NULL, async_thread, t)) {
		goto PTHREAD_CREATE_FAIL;
	}

	dbgln("%p thread=%ld waiting for startup", (void*)t, (unsigned long)t->thread);
	{
		const struct timespec timeout = ASYNC_THREAD_STARTUP_TIME_TIMESPEC;
		struct timespec abstime;
		try(clock_gettime(CLOCK_REALTIME, &abstime) == 0);
		abstime.tv_sec += timeout.tv_sec;
		abstime.tv_nsec += timeout.tv_nsec;
		const int ret = pthread_mutex_timedlock(&t->mutex, &abstime);
		if (ret != 0) {
			goto PTHREAD_MUTEX_TIMEDLOCK_FAIL;
		}
	}
	try(async_mutex_unlock(t) == 0);
	dbgln("%p thread=%ld started up", (void*)t, (unsigned long)t->thread);

	return 0;

	PTHREAD_MUTEX_TIMEDLOCK_FAIL:
	try(pthread_cancel(t->thread) == 0);
	try(pthread_detach(t->thread) == 0);
	PTHREAD_CREATE_FAIL:
	try(async_mutex_unlock(t) == 0);
	async_ref_down(t);
	return -1;
}

/* Alloc and Free Functions ------------------------------------------------------------- */

async_t *async_create_ex(enum async_policy_e policy, enum async_type_e type, union async_call_u call)
{
	assert((policy & ASYNC_POLICY_ASYNC) != 0 || (policy & ASYNC_POLICY_DEFERRED) != 0);
	assert(0 <= type && type <= (int)ASYNC_TYPE_MAX);
	switch(type) {
	case ASYNC_TYPE_VOID:
		assert(call.v.f != NULL);
		break;
	case ASYNC_TYPE_THEN:
		assert(call.t.f != NULL);
		assert(call.t.arg != NULL);
		break;
	case ASYNC_TYPE_WHENALL:
	case ASYNC_TYPE_WHENANY:
		assert(call.w.f != NULL);
		for (size_t i = 0; i < call.w.asyncs_cnt; ++i) {
			assert(call.w.asyncs[i] != NULL);
		}
		break;
	}

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

	t->refcnt = 1;
	t->policy = policy;
	t->type = type;
	t->c = call;
	if (t->type == ASYNC_TYPE_WHENALL || t->type == ASYNC_TYPE_WHENANY) {
		// copy the array call.w.asyncs
		async_t **asyncs = calloc(call.w.asyncs_cnt, sizeof(*asyncs));
		if (asyncs == NULL) {
			goto ASYNCS_CALLOC_FAIL;
		}
		memcpy(asyncs, call.w.asyncs, call.w.asyncs_cnt * sizeof(*asyncs));
		t->c.w.asyncs = asyncs;
	}

	async_refref_up(t);

	if ((t->policy & ASYNC_POLICY_ASYNC) != 0) {
		if (async_start_pthread(t)) {
			goto ASYNC_START_FAIL;
		}
	}

	return t;

	ASYNC_START_FAIL:
	async_refref_down(t);
	ASYNCS_CALLOC_FAIL:
	if (t->type == ASYNC_TYPE_WHENALL || t->type == ASYNC_TYPE_WHENANY) {
		free(t->c.w.asyncs);
	}
	(void)async_mutex_unlock(t);
	try(pthread_mutex_destroy(&t->mutex) == 0);
	PTHREAD_MUTEX_INIT_MUTEX_FAIL:
	try(close(t->readyfd) == 0);
	EVENTFD_FAIL:
	free(t);
	MALLOC_FAIL:
	dbgln("fail");
	return NULL;
}

static void async_free(async_t *t)
{
	assert(t != NULL);
	if (t->cleanup) {
		t->cleanup(t);
	}
	async_refref_down(t);
	if (t->type == ASYNC_TYPE_WHENALL || t->type == ASYNC_TYPE_WHENANY) {
		free(t->c.w.asyncs);
	}
	try(pthread_mutex_destroy(&t->mutex) == 0);
	try(close(t->readyfd) == 0);
	free(t);
	return;
}

/* Chain Functions --------------------------------------------------------------- */

async_t *async_create_chain(void *(*f)(void*), void *arg, ...)
{
	va_list va;
	va_start(va, arg);
	async_t *t = async_create_chain_va(f, arg, va);
	va_end(va);
	return t;
}

async_t *async_then_chain(async_t *t, void *(*f)(async_t *), ...)
{
	va_list va;
	va_start(va, f);
	t = async_then_chain_va(t, f, va);
	va_end(va);
	return t;
}

static async_t *async_chain_in(async_t *t, va_list va)
{
	if (t == NULL) {
		return NULL;
	}
	void *(*f)(async_t*);
	while ((f = va_arg(va, void *(*)(async_t*))) != NULL) {
		async_t * const newt = async_then(t, f);
		dbgln(" %p = async_then(%p, %p)", (void*)newt, (void*)t, (void*)(uintptr_t)f);
		if (newt == NULL) {
			async_cancel(&t);
			return NULL;
		}
		async_detach(&t);
		t = newt;
	}
	return t;
}

async_t *async_create_chain_va(void *(*f)(void*), void *arg, va_list va)
{
	async_t *t = async_create(f, arg);
	dbgln(" %p = async_create(%p, %p)", (void*)t, (void*)(uintptr_t)f, arg);
	return async_chain_in(t, va);
}

async_t *async_then_chain_va(async_t *t, void *(*f)(async_t *), va_list va)
{
	async_t *t2 = async_then(t, f);
	dbgln(" %p = async_then(%p, %p)", (void*)t, (void*)t, (void*)(uintptr_t)f);
	return async_chain_in(t2, va);
}


/* Destructors -------------------------------------------------------- */

void async_detach(async_t **t0)
{
	assert(t0 != NULL);
	async_t *t = *t0;
	assert(t != NULL);
	dbgln("%p ref=%u", (void*)t, t->refcnt);
	async_ref_down(t);
	*t0 = NULL;
}

void async_cancel(async_t **t0)
{
	assert(t0 != NULL);
	async_t *t = *t0;
	assert(t != NULL);
	dbgln("%p ref=%u", (void*)t, t->refcnt);
	try(async_mutex_lock(t) == 0);
	if ((t->policy & ASYNC_POLICY_ASYNC) != 0 && !async_is_ready(t)) {
		try(pthread_cancel(t->thread) == 0);
	}
	if (!async_ref_down_in(t)) {
		try(async_mutex_unlock(t) == 0);
	}
	*t0 = NULL;
}

/* Methods ------------------------------------------------------------------- */

int async_is_ready(async_t *t)
{
	assert(t != NULL);
	const int ret = async_wait_for(t, 0);
	dbgln("%p %sready", (void*)t, ret ? "" : "not ");
	return ret;
}

void *async_get(async_t *t)
{
	assert(t != NULL);
	dbgln("%p thread=%lu", (void*)t, (unsigned long)t->thread);
	try(async_wait_for(t, -1) == 1);
	try(async_mutex_lock(t) == 0);
	void * const ret = t->returned;
	try(async_mutex_unlock(t) == 0);
	dbgln("%p thread=%lu returned %p", (void*)t, (unsigned long)t->thread, ret);
	return ret;
}

int async_wait(async_t *t)
{
	const int ret = async_wait_for(t, -1);
	assert(ret < 0 || ret == 1);
	return ret;
}

int async_wait_for(async_t *t, int timeout)
{
	assert(t != NULL);
	if (timeout == -1 && ((t->policy & ASYNC_POLICY_DEFERRED) != 0) && !async_is_ready(t)) {
		dbgln("%p deferred executing", (void*)t);
		async_task_execute(t);
		async_notify_ready(t);
	}
    struct pollfd fds[1];
    fds[0].events = POLLIN;
    fds[0].fd = t->readyfd;
    return async_poll_ignore_signal(fds, 1, timeout);
}

int async_cleanup_reg(async_t *t, void (*cleanup_func)(async_t *))
{
	assert(t != NULL);
	assert(cleanup_func != NULL);
	tryret(async_mutex_lock(t), -1);
	if (!async_is_ready(t)) {
		t->cleanup = cleanup_func;
	} else {
		cleanup_func(t);
	}
	tryret(async_mutex_unlock(t), -1);
	return 0;
}

/* Asyncs -------------------------------------------------------------- */

void asyncs_detach(async_t *asyncs[], size_t asyncs_cnt)
{
	dbgln(" %p %zu", (void*)asyncs, asyncs_cnt);
	for (size_t i = 0 ; i < asyncs_cnt; ++i) {
		async_detach(&asyncs[i]);
	}
}

void asyncs_cancel(async_t *asyncs[], size_t asyncs_cnt)
{
	dbgln(" %p %zu", (void*)asyncs, asyncs_cnt);
	for (size_t i = 0 ; i < asyncs_cnt; ++i) {
		async_cancel(&asyncs[i]);
	}
}

int asyncs_wait(async_t *asyncs[], size_t asyncs_cnt)
{
	dbgln(" %p %zu", (void*)asyncs, asyncs_cnt);
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		const int ret = async_wait(asyncs[i]);
		if (ret <= 0) {
			return ret;
		}
	}
	return 0;
}

int asyncs_wait_for(async_t *asyncs[], size_t asyncs_cnt, int timeout)
{
	dbgln(" %p %zu", (void*)asyncs, asyncs_cnt);
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		const int ret = async_wait_for(asyncs[i], timeout);
		if (ret <= 0) {
			return ret;
		}
	}
	return 0;
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
	if (errno) {
		char buf[50];
		strerror_r(errno, buf, sizeof(buf));
		fprintf(stderr, "errno=%d %s\n", errno, buf);
	}
}

