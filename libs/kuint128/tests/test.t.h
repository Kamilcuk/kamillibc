#include "../src/kuint128.h"
#include <stdio.h>
#include <kuint128.h>
#include <stdlib.h>
#include <string.h>

#ifdef __AVR__
#define fprintf(...)
#endif

#define TEST(a, b)  void a##_##b(void)
#define RUNTEST(a, b)  a##_##b()
#define EXPECT(expr) \
	do{ if (!(expr)) { fprintf(stderr, "%s:%s:%d: failed %s\n", __FILE__, __func__, __LINE__, #expr); exit(-1); } }while(0)
#define EXPECT_EQ(a, b) \
	EXPECT(kuint128_eq(a, b))


