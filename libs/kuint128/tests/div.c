#include "test.t.h"

TEST(Arithmetic, divide){
    const kuint128 big_val  = kuint128_init_u(0xfedbca9876543210ULL);
    const kuint128 small_val= kuint128_init_u(0xffffULL);
    // const kuint128 res_val  = kuint128_init_u(0xfedcc9753fc9ULL);

    EXPECT_EQ(kuint128_div(small_val, small_val), kuint128_init_u(1));
    EXPECT_EQ(kuint128_div(small_val, big_val),   kuint128_init_u(0));
    EXPECT_EQ(kuint128_div(big_val, big_val),   kuint128_init_u(1));
}

TEST(External, divide){
    kuint128 t = kuint128_init_u(true);
    kuint128 f = kuint128_init_u(false);
    kuint128 u8 = kuint128_init_u(0xaaULL);
    kuint128 u16 = kuint128_init_u(0xaaaaULL);
    kuint128 u32 = kuint128_init_u(0xaaaaaaaaULL);
    kuint128 u64 = kuint128_init_u(0xaaaaaaaaaaaaaaaaULL);

    const kuint128 val = kuint128_init_u(0x7bULL);

    EXPECT_EQ(kuint128_div(t, val), kuint128_init_u(false));
    EXPECT_EQ(kuint128_div(f, val), kuint128_init_u(false));

	kuint128_println(u8);
	kuint128_println(val);
	kuint128_println(kuint128_div(u8, val));
	kuint128_println(kuint128_mod(u8, val));
    EXPECT_EQ(kuint128_div(u8, val), kuint128_init_u(0x1ULL));
    EXPECT_EQ(kuint128_div(u16, val), kuint128_init_u(0x163ULL));
    EXPECT_EQ(kuint128_div(u32, val), kuint128_init_u(0x163356bULL));
    EXPECT_EQ(kuint128_div(u64, val), kuint128_init_u(0x163356b88ac0de0ULL));

    EXPECT(kuint128_bool(kuint128_idiv(&t, val)) == false);
    EXPECT(kuint128_bool(kuint128_idiv(&f, val)) == false);
    EXPECT(kuint128_uint8(kuint128_idiv(&u8, val)) == (uint8_t)  0x1ULL);
    EXPECT(kuint128_uint16(kuint128_idiv(&u16, val)) == (uint16_t) 0x163ULL);
    EXPECT(kuint128_uint32(kuint128_idiv(&u32, val)) == (uint32_t) 0x163356bULL);
    EXPECT(kuint128_uint64(kuint128_idiv(&u64, val)) == (uint64_t) 0x163356b88ac0de0ULL);
}

int main() {
	Arithmetic_divide();
	External_divide();
}
