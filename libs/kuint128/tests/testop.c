#pragma GCC diagnostic ignored "-Wmultichar"
#define KUINT128_WANT_OP  1
#include "test.t.h"
#include <curb.h>
#include <stdio.h>

#define TES(a, b)  curb_exit(kuint128_eq(kuint128_op(kuint128_op a), kuint128_op b))
static inline void _kuint128_op_example_usage(void) {
	kuint128 v = kuint128_op("1000");
	TES( ('++', &v), (1001) );
	TES( ('--', &v), (1000) );
	TES( ('!', v), (0) );
	TES( ('!!', v), (1) );
	TES( kuint128_op('+', v), (v) );
	TES( kuint128_op('-', v), (-1000) );
	kuint128 x = kuint128_op("100");
	TES( kuint128_op(v, '+', x), (1100) );
	TES( kuint128_op(v, '-', x), (900) );
	TES( kuint128_op(v, '*', x), (100000) );
	TES( kuint128_op(v, '/', x), (10) );
	v = kuint128_op(0x101);
	x = kuint128_op(0x110);
	TES( kuint128_op(v, '|', x), (0x111) );
	TES( kuint128_op(v, '&', x), (0x100) );
	TES( kuint128_op(v, '^', x), (0x011) );
}

int main() {
	_kuint128_op_example_usage();
}

