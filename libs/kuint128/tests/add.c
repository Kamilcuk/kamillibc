#include "test.t.h"

TEST(Arithmetic, add) {
	kuint128 low = kuint128_init(0, 1);
	kuint128 high = kuint128_init(1, 0);

	EXPECT_EQ(kuint128_add(low, low), kuint128_init_u(2));
	EXPECT_EQ(kuint128_add(low, high), kuint128_init(1, 1));
	EXPECT_EQ(kuint128_add(high, high), kuint128_init(2, 0));

	EXPECT_EQ(kuint128_iadd(&low, low), kuint128_init_u(2));
	EXPECT_EQ(kuint128_iadd(&low, high), kuint128_init(1, 2));
	EXPECT_EQ(kuint128_iadd(&high, low), kuint128_init(2, 2));
}

TEST(External, add) {
	kuint128 t = kuint128_init_u(true);
	kuint128 f = kuint128_init_u(false);
	kuint128 u8 = kuint128_init_u(0xaaULL);
	kuint128 u16 = kuint128_init_u(0xaaaaULL);
	kuint128 u32 = kuint128_init_u(0xaaaaaaaaULL);
	kuint128 u64 = kuint128_init_u(0xaaaaaaaaaaaaaaaaULL);

	const kuint128 val = kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

	EXPECT_EQ(kuint128_add(t, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
	EXPECT_EQ(kuint128_add(f, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
	EXPECT_EQ(kuint128_add(u8, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f19aULL));
	EXPECT_EQ(kuint128_add(u16, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f19b9aULL));
	EXPECT_EQ(kuint128_add(u32, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f19b9b9b9aULL));
	EXPECT_EQ(kuint128_add(u64, val),
		  kuint128_init(0xf0f0f0f0f0f0f0f1ULL, 0x9b9b9b9b9b9b9b9aULL));
}

int main() {
	RUNTEST(Arithmetic, add);
	RUNTEST(External, add);
}
