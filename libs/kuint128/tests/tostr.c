#include "test.t.h"

#define EXPECT_STRCMP(a, b) \
	do { if (strcmp(a, b)) { fprintf(stderr, "%s:%s:%d: strcmp(%s, %s) failed\n\t%s\n\t%s\n", __FILE__, __func__, __LINE__, #a, #b, a, b); exit(1); } }while(0)

int main() {
	EXPECT_STRCMP(KUINT128_STR(kuint128_max), "340282366920938463463374607431768211455");
	EXPECT_STRCMP(KUINT128_HEXSTR(kuint128_max), "ffffffffffffffffffffffffffffffff");
	EXPECT_STRCMP(KUINT128_OCTSTR(kuint128_max), "3777777777777777777777777777777777777777777");
	EXPECT_STRCMP(KUINT128_BINSTR(kuint128_max), "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

	EXPECT_STRCMP(KINT128_STR(kuint128_max), "-1");
	EXPECT_STRCMP(KINT128_HEXSTR(kuint128_max), "-1");
	EXPECT_STRCMP(KINT128_OCTSTR(kuint128_max), "-1");
	EXPECT_STRCMP(KINT128_BINSTR(kuint128_max), "-1");

	EXPECT_STRCMP(KINT128_STR(kint128_max), "170141183460469231731687303715884105727");
	EXPECT_STRCMP(KINT128_HEXSTR(kint128_max), "7fffffffffffffffffffffffffffffff");
	EXPECT_STRCMP(KINT128_OCTSTR(kint128_max), "1777777777777777777777777777777777777777777");
	EXPECT_STRCMP(KINT128_BINSTR(kint128_max), "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

	EXPECT_STRCMP(KINT128_STR(kint128_min), "-170141183460469231731687303715884105728");
	EXPECT_STRCMP(KINT128_HEXSTR(kint128_min), "-80000000000000000000000000000000");
	EXPECT_STRCMP(KINT128_OCTSTR(kint128_min), "-2000000000000000000000000000000000000000000");
	EXPECT_STRCMP(KINT128_BINSTR(kint128_min), "-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}
