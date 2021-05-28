#include "test.t.h"

TEST(Arithmetic, multiply){
    kuint128 val = kuint128_init_u(0xfedbca9876543210ULL);

    EXPECT_EQ(kuint128_mul(val, val), kuint128_init(0xfdb8e2bacbfe7cefULL, 0x010e6cd7a44a4100ULL));

    const kuint128 zero = kuint128_0;
    EXPECT_EQ(kuint128_mul(val, zero), zero);
    EXPECT_EQ(kuint128_mul(zero, val), zero);

    const kuint128 one = kuint128_1;
    EXPECT_EQ(kuint128_mul(val, one), val);
    EXPECT_EQ(kuint128_mul(one, val), val);
}

TEST(External, multiply){
	kuint128 t = kuint128_init_u(true);
	kuint128 f = kuint128_init_u(false);
	kuint128 u8 = kuint128_init_u(0xaaULL);
	kuint128 u16 = kuint128_init_u(0xaaaaULL);
	kuint128 u32 = kuint128_init_u(0xaaaaaaaaULL);
	kuint128 u64 = kuint128_init_u(0xaaaaaaaaaaaaaaaaULL);

    const kuint128 val = kuint128_init(0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0ULL);

    EXPECT_EQ(kuint128_mul(t, val), val);
    EXPECT_EQ(kuint128_mul(f, val), kuint128_0);
    EXPECT_EQ(kuint128_mul(u8, val), kuint128_init(0xffffffffffffffff, 0xffffffffffffff60ULL));
    EXPECT_EQ(kuint128_mul(u16, val), kuint128_init(0xffffffffffffffff, 0xffffffffffff5f60ULL));
    EXPECT_EQ(kuint128_mul(u32, val), kuint128_init(0xffffffffffffffff, 0xffffffff5f5f5f60ULL));
    EXPECT_EQ(kuint128_mul(u64, val), kuint128_init(0xffffffffffffffff, 0x5f5f5f5f5f5f5f60ULL));

    EXPECT(kuint128_bool(kuint128_imul(&t, val)) == true);
    EXPECT(kuint128_bool(kuint128_imul(&f, val)) == false);
    EXPECT(kuint128_uint8(kuint128_imul(&u8, val)) == 0x60);
    EXPECT(kuint128_uint16(kuint128_imul(&u16, val)) == 0x5f60ULL);
    EXPECT(kuint128_uint32(kuint128_imul(&u32, val)) == 0x5f5f5f60ULL);
    EXPECT(kuint128_uint64(kuint128_imul(&u64, val)) == 0x5f5f5f5f5f5f5f60ULL);
}

int main() {
	Arithmetic_multiply();
	External_multiply();
}
