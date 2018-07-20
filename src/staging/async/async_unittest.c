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

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void *f_ret_1(void *arg) {
	(void)arg;
	usleep(100);
	return (void*)(uintptr_t)1;
}
static void *f_return_any(async_t *asyncs[], size_t asyncs_cnt) {
	usleep(400);
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		if (async_is_ready(asyncs[i])) {
			return async_get(asyncs[i]);
		}
	}
	return NULL;
}
static void *f_sum(async_t *asyncs[], size_t asyncs_cnt) {
	usleep(500);
	int sum = 0;
	for (size_t i = 0; i < asyncs_cnt; ++i) {
		sum += (int)(uintptr_t)async_get(asyncs[i]);
	}
	return (void*)(uintptr_t)sum;
}
static long long sec_to_ms(long long s) {
	return s * 1000ll;
}
static long long us_to_ms(long long us) {
	return us / 1000ll;
}
static long long timeval_sub_ms(struct timeval *tm1, struct timeval *tm2) {
	return sec_to_ms(tm2->tv_sec - tm1->tv_sec) + us_to_ms(tm2->tv_usec - tm1->tv_usec);
}
static void *f_usleep_100ms(void *arg) {
	usleep(100*1000);
	return (void*)(uintptr_t)1;
}
static struct timeval measure_start(void) {
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return tm;
}
static long long measure_stop_ms(struct timeval tm1) {
	struct timeval tm2;
	gettimeofday(&tm2, NULL);
	return timeval_sub_ms(&tm1, &tm2);
}
static void *f_return_1(void *arg) {
	(void)arg;
	return (void*)(uintptr_t)1;
}
static void *f_return_arg(void *arg) {
	return arg;
}
static void *f_return_arg_async(async_t *arg) {
	return arg;
}
static void *f_mul_2(async_t *arg) {
	return (void*)((uintptr_t)async_get(arg) * 2);
}
static void *f_add_2(async_t *arg) {
	return (void*)((uintptr_t)async_get(arg) + 2);
}

typedef struct {
	long usleeptime;
	uintptr_t retval;
} args1_t;

static void *f_args1(void *arg0) {
	args1_t *t = arg0;
	assert(t != NULL);
	usleep(t->usleeptime);
	return (void*)t->retval;
}


int async_unittest(void) {
#define TEST(expr) do{ if (!(expr)) { fprintf(stderr, "%s: %s failed!\n", __func__, #expr); return -__LINE__; } }while(0)
	struct timeval tm;
	{
		tm = measure_start();
		async_t *t = async_create(f_usleep_100ms, NULL);
		TEST(async_is_ready(t) == false);
		TEST(async_is_ready(t) == false);
		usleep(100*1000/2);
		TEST(async_is_ready(t) == false);
		TEST(async_is_ready(t) == false);
		TEST((uintptr_t)async_get(t) == 1);
		//printf("%lld\n", measure_stop_ms(tm));
		TEST(100 <= measure_stop_ms(tm) && measure_stop_ms(tm) <= 200); // 100 normally, 161 under valgrind
		TEST(async_is_ready(t) == true);
		TEST(async_is_ready(t) == true);
		TEST((uintptr_t)async_get(t) == 1);
		async_cancel(&t);
	}
	{
		tm = measure_start();
		async_t *t[] = {
				async_create(f_return_1, NULL),
				async_then(t[0], f_mul_2),
				async_then(t[1], f_mul_2),
		};
		TEST((uintptr_t)async_get(t[2]) == 4);
		TEST(0 <= measure_stop_ms(tm) && measure_stop_ms(tm) <= 100); // 0 normally, 79 under valgrind
		asyncs_cancel(t, sizeof(t)/sizeof(t[0]));
	}
	{
		async_t *t = async_create_arg_self(f_return_arg_async);
		TEST(async_get(t) == t);
		async_cancel(&t);
	}
	{
		async_t *t[] = {
				async_create(f_args1, &(args1_t){100000, 1}),
				async_create(f_args1, &(args1_t){200000, 2}),
				async_create(f_args1, &(args1_t){300000, 3}),
				async_create(f_args1, &(args1_t){400000, 4}),
				async_when_any(t, 4, f_return_any),
				async_when_all(t, 4, f_sum),
		};
		TEST((uintptr_t)async_get(t[4]) == 1);
		TEST((uintptr_t)async_get(t[5]) == 1 + 2 + 3 + 4);
		asyncs_cancel(t, sizeof(t)/sizeof(t[0]));
	}
	return 0;
}

int main(void) {
	printf("async_test = %d\n", async_unittest());
	return 0;
}
