#include "test.t.h"

TEST(BitShift, left) {
	// operator<<
	kuint128 val = kuint128_init_u(0x1);
	kuint128 exp_val = kuint128_init_u(1);
	for (kuint128 i = kuint128_init_u(0); kuint128_lt_u(i, 64); kuint128_inc(&i)) {
		EXPECT_EQ(kuint128_lshift(val, i), kuint128_lshift(exp_val, i));
	}

	kuint128 zero = kuint128_0;
	for (kuint128 i = kuint128_init_u(0); kuint128_lt_u(i, 64); kuint128_inc(&i)) {
		EXPECT_EQ(kuint128_lshift(zero, i), zero);
	}

	// operator<<=
	for (kuint128 i = kuint128_init_u(0); kuint128_lt_u(i, 64); kuint128_inc(&i)) {
		EXPECT_EQ(kuint128_ilshift_u(&val, 1), kuint128_ilshift_u(&exp_val, 1));
	}

	for (kuint128 i = kuint128_init_u(0); kuint128_lt_u(i, 64); kuint128_inc(&i)) {
		EXPECT_EQ(kuint128_ilshift_u(&zero, 1), zero);
	}
}

TEST(External, shift_left) {
	kuint128 t = kuint128_init_u(true);
	kuint128 f = kuint128_init_u(false);
	kuint128 u8 = kuint128_init_u(0xffULL);
	kuint128 u16 = kuint128_init_u(0xffffULL);
	kuint128 u32 = kuint128_init_u(0xffffffffULL);
	kuint128 u64 = kuint128_init_u(0xffffffffffffffffULL);

	const kuint128 zero = kuint128_0;
	const kuint128 one = kuint128_1;

	EXPECT_EQ(kuint128_lshift(t, zero), t);
	EXPECT_EQ(kuint128_lshift(f, zero), f);
	EXPECT_EQ(kuint128_lshift(u8, zero), u8);
	EXPECT_EQ(kuint128_lshift(u16, zero), u16);
	EXPECT_EQ(kuint128_lshift(u32, zero), u32);
	EXPECT_EQ(kuint128_lshift(u64, zero), u64);

	EXPECT_EQ(kuint128_ilshift(&t, zero), t);
	EXPECT_EQ(kuint128_ilshift(&f, zero), f);
	EXPECT_EQ(kuint128_ilshift(&u8, zero), u8);
	EXPECT_EQ(kuint128_ilshift(&u16, zero), u16);
	EXPECT_EQ(kuint128_ilshift(&u32, zero), u32);
	EXPECT_EQ(kuint128_ilshift(&u64, zero), u64);

	EXPECT_EQ(kuint128_lshift(t, one), kuint128_lshift_u(t, 1));
	EXPECT_EQ(kuint128_lshift(f, one), kuint128_lshift_u(f, 1));
	EXPECT_EQ(kuint128_lshift(u8, one), kuint128_lshift_u(u8, 1));
	EXPECT_EQ(kuint128_lshift(u16, one), kuint128_lshift_u(u16, 1));
	EXPECT_EQ(kuint128_lshift(u32, one), kuint128_lshift_u(u32, 1));
	EXPECT_EQ(kuint128_lshift(u64, one), kuint128_lshift_u(u64, 1));

	EXPECT(kuint128_bool(kuint128_ilshift(&t, one)) == true);
	EXPECT(kuint128_bool(kuint128_ilshift(&f, one)) == false);
	EXPECT(kuint128_uint8(kuint128_ilshift(&u8, one)) == (uint8_t)0xfeULL);
	EXPECT(kuint128_uint16(kuint128_ilshift(&u16, one)) == (uint16_t)0xfffeULL);
	EXPECT(kuint128_uint32(kuint128_ilshift(&u32, one)) == (uint32_t)0xfffffffeULL);
	EXPECT(kuint128_uint64(kuint128_ilshift(&u64, one)) == (uint64_t)0xfffffffffffffffeULL);

	// sorry, but not auto-assignment-casting in C
	u8 = kuint128_init_u(kuint128_uint8(u8));
	u16 = kuint128_init_u(kuint128_uint16(u16));
	u32 = kuint128_init_u(kuint128_uint32(u32));
	u64 = kuint128_init_u(kuint128_uint64(u64));

	EXPECT_EQ(kuint128_lshift(u8, kuint128_init_u(7)), kuint128_init_u(0x7f00ULL));
	EXPECT_EQ(kuint128_lshift(u16, kuint128_init_u(15)), kuint128_init_u(0x7fff0000ULL));
	EXPECT_EQ(kuint128_lshift(u32, kuint128_init_u(31)), kuint128_init_u(0x7fffffff00000000ULL));
	EXPECT_EQ(kuint128_lshift(u64, kuint128_init_u(63)),
		  kuint128_init(0x7fffffffffffffff, 0x0000000000000000ULL));

	EXPECT(kuint128_uint8(kuint128_ilshift(&u8, kuint128_init_u(7))) == 0);
	EXPECT(kuint128_uint16(kuint128_ilshift(&u16, kuint128_init_u(15))) == 0);
	EXPECT(kuint128_uint32(kuint128_ilshift(&u32, kuint128_init_u(31))) == 0);
	EXPECT(kuint128_uint64(kuint128_ilshift(&u64, kuint128_init_u(63))) == 0);
}

int main() {
	BitShift_left();
	External_shift_left();
}
