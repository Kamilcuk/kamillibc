#include "test.t.h"

int main() {
	for (int_fast64_t i = -1; i > -100000000000ll; i -= 10000000000ll) {
		/*0&&printf("%s = %s || %s\n",
				KINT128_HEXSTR(kuint128_init_d(i)),
				KINT128_STR(kuint128_init_d(i)),
				KINT128_STR(kuint128_init_u(i)));*/
		EXPECT_EQ(kuint128_init_d(i), kuint128_sub(kuint128_max, kuint128_init_u(-i - 1)));
		EXPECT_EQ(kuint128_init_d(i), kuint128_unsigned_to_signed(kuint128_init_u(~i + 1)));
		EXPECT_EQ(kuint128_signed_to_unsigned(kuint128_init_d(i)), kuint128_init_u(~i + 1));
	}
}

