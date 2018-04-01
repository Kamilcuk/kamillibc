/*
 * algorithm.c
 *
 *  Created on: 01.04.2018
 *      Author: michal
 */
#include <cdefs.h>

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <cplusplus/algorithm.h>
#include <stdio.h>
#include <stdlib.h>

struct t_s {
	int a;
	int b;
};
typedef struct t_s * typedefpnt;

static ALGO_H_find(TESTfind_, const int *);
static ALGO_C_find(TESTfind_, const int *)

static ALGO_H_find(TESTfind2_, typedefpnt);
static __unused ALGO_C_find(TESTfind2_, typedefpnt)

static ALGO_H_for_each(TEST_, const int *);
static ALGO_C_for_each(TEST_, const int *)

static ALGO_H_search1(TEST1_, const int *);
static ALGO_C_search1(TEST1_, const int *)

static ALGO_H_search2(TEST2_, const int, const int);
static ALGO_C_search2(TEST2_, const int, const int)

static ALGO_H_transform1(T_, const int, char);
static ALGO_C_transform1(T_, const int, char)

static ALGO_H_transform2(T_, const int, const int, char);
static ALGO_C_transform2(T_, const int, const int, char)

int algo_unittest()
{
#define TEST(expr) do{ if (!(expr)) { assert(expr); return -__LINE__; } }while(0)
	{
		const int a[] = {1,2,3,4};
		const int b = 3;
		const int * const it = TESTfind_find(a, a+4, &b);
		TEST(it == a+2);
	}
	{
		const int a[] = {1,2,3,4};
		int sum = 0;
		void fn(const int *el, void *arg0) {
			int * sum = arg0;
			*sum += *el;
		}
		TEST_for_each(a, a+4, fn, &sum);
		TEST(sum == 10);
	}
	{
		const int a[] = {1,2,3,4};
		const int b[] = {2,3};
		const int * const it = TEST1_search1(a, a+4, b, b+2);
		TEST(it == a+1);
	}
	{
		bool mypredicate (const int *i, const int *j, void *arg) {
		  return *i == *j;
		}
		const int a[] = {1,2,3,4};
		const int b[] = {2,3};
		const int * const it = TEST2_search2(a, a+4, b, b+2, mypredicate, NULL);
		TEST(it == a+1);
	}
	{
		const int a[] = {1,2,3,4};
		char buf[5];
		char op_transform1(const int *el1, void *arg) {
			return *el1+'0';
		}
		T_transform1(a, a+4, buf, op_transform1, NULL);
		TEST(!memcmp(buf, "1234", 4));
	}
	{
		const int a[] = {1,2,3,4};
		const int b[] = {0,1,2,3};
		char buf[5];
		char op_transform2(const int *el1, const int *el2, void *arg) {
			return *el1+*el2+'0';
		}
		T_transform2(a, a+4, b, buf, op_transform2, NULL);
		TEST(!memcmp(buf, "1357", 4));
	}
	return 0;
}

