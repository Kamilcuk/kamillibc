#include "test.t.h"

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
uint32_t how_to_split_double_multiplication(double x) {
	kuint128 y = kuint128_init_u(x * 10000);
	kuint128 c = kuint128_init_u(8813377768984962ull);
	kuint128 tmp = y;
	kuint128_imul(&tmp, y);
	kuint128_imul(&tmp, c);
	kuint128_idiv(&tmp, kuint128_init_u(10000));
	kuint128_idiv(&tmp, kuint128_init_u(10000));
	kuint128_idiv(&tmp, kuint128_init_u(1000000000ull));
	return kuint128_uint32(tmp);
}

int main() {
	uint32_t r = how_to_split_double_multiplication(9.6103);
	EXPECT(r == 813984763);
}
