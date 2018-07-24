/*
 * async_unittest.c
 *
 *  Created on: 17 lip 2018
 *      Author: Kamil Cukrowski
 *     License: Jointly under MIT License and Beerware License
 */

#define _GNU_SOURCE
#include "async.h"
#include <assert.h>
#include <stddef.h>
#include <poll.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

static void *f_ret_1(void *arg) {
	(void)arg;
	return (void*)(uintptr_t)1;
}
static void *f_return_any(async_t *asyncs[], size_t asyncs_cnt) {
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		if (async_is_ready(asyncs[i])) {
			return async_get(asyncs[i]);
		}
	}
	return NULL;
}
static void *f_sum(async_t *asyncs[], size_t asyncs_cnt) {
	int sum = 0;
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		sum += (int)(uintptr_t)async_get(asyncs[i]);
	}
	return (void*)(uintptr_t)sum;
}
static long long sec_to_ms(long long s) {
	return s * 1000ll;
}
static long long ns_to_ms(long long us) {
	return us / 1000000ll;
}
static long long timespec_sub_ms(struct timespec *tm1, struct timespec *tm2) {
	return sec_to_ms(tm2->tv_sec - tm1->tv_sec) + ns_to_ms(tm2->tv_nsec - tm1->tv_nsec);
}
static void *f_lock_mutex(void *arg) {
	pthread_mutex_t *mutex = arg;
	pthread_mutex_lock(mutex);
	pthread_mutex_unlock(mutex);
	return (void*)(uintptr_t)1;
}
static struct timespec measure_start(void) {
	struct timespec tm;
	clock_gettime(CLOCK_MONOTONIC, &tm);
	return tm;
}
static long long measure_stop_ms(struct timespec tm1) {
	struct timespec tm2;
	clock_gettime(CLOCK_MONOTONIC, &tm2);
	return timespec_sub_ms(&tm1, &tm2);
}
static void *f_mul_2_void(void *arg) {
	return (void*)((uintptr_t)arg * 2);
}
static void *f_return_1(void *arg) {
	(void)arg;
	return (void*)(uintptr_t)1;
}
static void *f_mul_2(async_t *arg) {
	return (void*)((uintptr_t)async_get(arg) * 2);
}
static void *f_add_2(async_t *arg) {
	return (void*)((uintptr_t)async_get(arg) + 2);
}

typedef struct {
	unsigned long usleeptime;
	uintptr_t retval;
} args1_t;

static void *f_args1(void *arg0) {
	args1_t *t = arg0;
	assert(t != NULL);
	usleep(t->usleeptime);
	return (void*)t->retval;
}

typedef struct {
	pthread_mutex_t mutex;
	uintptr_t retval;
} args2_t;

static void *f_args2(void *arg0) {
	args2_t *t = arg0;
	assert(t != NULL);
	pthread_mutex_lock(&t->mutex);
	pthread_mutex_unlock(&t->mutex);
	return (void*)t->retval;
}


//#define dbgln(str, ...) ((void)0)
#ifndef dbgln
#define dbgln(str, ...)  fprintf(stderr, str "\n", ##__VA_ARGS__)
#endif
#define TEST(expr) do{ if (!(expr)) { fprintf(stderr, "%s:%d: %s failed!\n", __func__, __LINE__, #expr); return -__LINE__; } }while(0)

int async_unittest(void) {
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	dbgln("-- %d ---------------------------------", __LINE__);
	for (int i = 0; i < 50; ++i) {
		TEST(pthread_mutex_lock(&mutex) == 0);
		async_t *t = async_create(f_lock_mutex, &mutex);
		TEST(t != NULL);
		pthread_mutex_unlock(&mutex);
		async_cancel(&t);
	}
	return 0;
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		TEST(pthread_mutex_lock(&mutex) == 0);
		async_t *t = async_create(f_lock_mutex, &mutex);
		TEST(t != NULL);
		async_cancel(&t);
		pthread_mutex_unlock(&mutex);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		TEST(pthread_mutex_lock(&mutex) == 0);
		async_t *t = async_create(f_lock_mutex, &mutex);
		TEST(t != NULL);
		async_detach(&t);
		pthread_mutex_unlock(&mutex);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		TEST(pthread_mutex_lock(&mutex) == 0);
		async_t *t = async_create(f_lock_mutex, &mutex);
		TEST(t != NULL);
		async_detach(&t);
		pthread_mutex_unlock(&mutex);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	for (int i = 0; i < 50; ++i) {
		async_t *t = async_create(f_mul_2_void, (void*)(uintptr_t)i);
		TEST(t != NULL);
		TEST((uintptr_t)async_get(t) == (uintptr_t)i * 2);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	for (int i = 0; i < 50; ++i) {
		async_t *t = async_create(f_mul_2_void, (void*)(uintptr_t)i);
		TEST(t != NULL);
		TEST((uintptr_t)async_get(t) == (uintptr_t)i * 2);
		async_detach(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		TEST(pthread_mutex_lock(&mutex) == 0);
		async_t *t = async_create(f_lock_mutex, &mutex);
		TEST(t != NULL);
		TEST(async_is_ready(t) == false);
		TEST(async_is_ready(t) == false);
		TEST(pthread_mutex_unlock(&mutex) == 0);
		TEST((uintptr_t)async_get(t) == 1);
		TEST(async_is_ready(t) == true);
		TEST(async_is_ready(t) == true);
		TEST((uintptr_t)async_get(t) == 1);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t[] = {
				async_create(f_return_1, NULL),
				async_then(t[0], f_mul_2),
				async_then(t[1], f_mul_2),
		};
		TEST((uintptr_t)async_get(t[2]) == 4);
		asyncs_cancel(t, sizeof(t)/sizeof(t[0]));
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		args2_t at[4] = {
			{ PTHREAD_MUTEX_INITIALIZER, 1 },
			{ PTHREAD_MUTEX_INITIALIZER, 2 },
			{ PTHREAD_MUTEX_INITIALIZER, 3 },
			{ PTHREAD_MUTEX_INITIALIZER, 4 },
		};
		for (size_t i = 0; i < sizeof(at)/sizeof(at[0]); ++i) {
			TEST(pthread_mutex_lock(&at[i].mutex) == 0);
		}
		async_t *t[] = {
				async_create(f_args2, &at[0]),
				async_create(f_args2, &at[1]),
				async_create(f_args2, &at[2]),
				async_create(f_args2, &at[3]),
				async_when_any(t, 4, f_return_any),
		};
		TEST(pthread_mutex_unlock(&at[1].mutex) == 0);
		TEST((uintptr_t)async_get(t[4]) == 2);
		for (size_t i = 0; i < sizeof(at)/sizeof(at[0]); ++i) {
			if (i != 1) {
				TEST(pthread_mutex_unlock(&at[i].mutex) == 0);
			}
			TEST((uintptr_t)async_get(t[i]) == i + 1);
			TEST(pthread_mutex_destroy(&at[i].mutex) == 0);
		}
		asyncs_cancel(t, sizeof(t)/sizeof(t[0]));
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t[] = {
				async_create(f_args1, &(args1_t){10000, 1}),
				async_create(f_args1, &(args1_t){20000, 2}),
				async_create(f_args1, &(args1_t){30000, 3}),
				async_create(f_args1, &(args1_t){40000, 4}),
				async_when_all(t, 4, f_sum),
		};
		TEST((uintptr_t)async_get(t[4]) == 1 + 2 + 3 + 4);
		asyncs_cancel(t, sizeof(t)/sizeof(t[0]));
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t[] = {
				async_create(f_args1, &(args1_t){400000, 4}),
				async_create(f_args1, &(args1_t){200000, 2}),
				async_create(f_args1, &(args1_t){300000, 3}),
				async_create(f_args1, &(args1_t){100000, 1}),
				async_when_any(t, 4, f_return_any),
		};
		TEST((uintptr_t)async_get(t[4]) == 1);
		asyncs_detach(t, sizeof(t)/sizeof(t[0]));
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t[] = {
				async_create(f_args1, &(args1_t){10000, 1}),
				async_create(f_args1, &(args1_t){20000, 2}),
				async_create(f_args1, &(args1_t){30000, 3}),
				async_create(f_args1, &(args1_t){40000, 4}),
				async_when_all(t, 4, f_sum),
		};
		TEST((uintptr_t)async_get(t[4]) == 1 + 2 + 3 + 4);
		asyncs_detach(t, sizeof(t)/sizeof(t[0]));
	}
	dbgln("-- %d ---------------------------------", __LINE__);

	async_t *t0 = async_create(f_ret_1, NULL);
	TEST(t0 != NULL);

	{
		async_t *t = async_create_chain(
				f_ret_1, (void*)(uintptr_t)1,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 1);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_then_chain(
				t0,
				f_mul_2,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 2);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_create(f_ret_1, (void*)(uintptr_t)1);
		async_t *t2 = async_then(t, f_mul_2);
		async_detach(&t);
		TEST((uintptr_t)async_get(t2) == 2);
		async_cancel(&t2);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_create_chain(
				f_ret_1, (void*)(uintptr_t)1,
				f_mul_2,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 2);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_create_chain(
				f_ret_1, (void*)(uintptr_t)1,
				f_mul_2,
				f_mul_2,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 4);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_then_chain(
				t0,
				f_mul_2,
				f_mul_2,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 4);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t = async_create_chain(
				f_ret_1, (void*)(uintptr_t)1,
				f_add_2,
				f_mul_2,
				f_mul_2,
				f_add_2,
				NULL
		);
		TEST((uintptr_t)async_get(t) == 14);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);
	{
		async_t *t1 = async_create(f_ret_1, NULL);
		TEST(t1 != NULL);
		async_t *t = async_then_chain(
				t1,
				f_add_2,
				f_mul_2,
				f_mul_2,
				f_add_2,
				NULL
		);
		TEST(t != NULL);
		TEST((uintptr_t)async_get(t) == 14);
		async_cancel(&t1);
		async_cancel(&t);
	}
	dbgln("-- %d ---------------------------------", __LINE__);

	async_cancel(&t0);
	TEST(pthread_mutex_destroy(&mutex) == 0);
	usleep(500*1000);

	return 0;
}

int main(void) {
	printf("async_test = %d\n", async_unittest());
	return 0;
}
