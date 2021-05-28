/**
An unsigned 128 bit integer type for C.
Written by Kamil Cukrowski
Licensed jointly under Beerware License and MIT License.

---------------------------------------------------------------------------------------

Based on uint128_t from https://github.com/calccrypto/uint128_t that is under MIT LICENSE

Copyright (c) 2013 - 2017 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

With much help from Auston Sterling

Thanks to Stefan Deigmüller for finding a bug in operator*.

Thanks to François Dessenne for convincing me
to do a general rewrite of this class.
 */
#include "kuint128.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

kuint128 kuint128_init_str_hex(const char *s, const char **endptr) {
	if (s == NULL || !s[0]) {
		if (*endptr)
			*endptr = s;
		return kuint128_init(0, 0);
	}
	kuint128 t = kuint128_init(0, 0);
	// 2**128 = 0x100000000000000000000000000000000.
	for (const char *end = s + 16; s < end && *s; ++s) {
		if ('0' <= *s && *s <= '9') {
			t.LOWER *= 16;
			t.LOWER += *s - '0';
		} else if ('A' <= *s && *s <= 'F') {
			t.LOWER *= 16;
			t.LOWER += *s + (10 - 'A');
		} else if ('a' <= *s && *s <= 'f') {
			t.LOWER *= 16;
			t.LOWER += *s + (10 - 'a');
		} else {
			return t;
		}
	}
	for (const char *end = s + 16; *s && s < end; ++s) {
		if ('0' <= *s && *s <= '9') {
			kuint128_imul_u(&t, 16);
			kuint128_iadd_u(&t, *s - '0');
		} else if ('A' <= *s && *s <= 'F') {
			kuint128_imul_u(&t, 16);
			kuint128_iadd_u(&t, *s + (10 - 'A'));
		} else if ('a' <= *s && *s <= 'f') {
			kuint128_imul_u(&t, 16);
			kuint128_iadd_u(&t, *s + (10 - 'a'));
		} else {
			break;
		}
	}
	if (*endptr)
		*endptr = s;
	return t;
}

kuint128 kuint128_init_str_dec(const char *s, const char **endptr) {
	if (s == NULL || !s[0]) {
		if (*endptr)
			*endptr = s;
		return kuint128_init(0, 0);
	}
	kuint128 t = kuint128_init(0, 0);
	// 2**128 = 340282366920938463463374607431768211456.
	for (const char *end = s + 39; '0' <= *s && *s <= '9' && s < end; ++s) {
		kuint128_imul_u(&t, 10);
		kuint128_iadd_u(&t, *s - '0');
	}
	if (*endptr)
		*endptr = s;
	return t;
}

kuint128 kuint128_init_str_oct(const char *s, const char **endptr) {
	if (s == NULL || !s[0]) {
		if (*endptr)
			*endptr = s;
		return kuint128_init(0, 0);
	}
	kuint128 t = kuint128_init(0, 0);
	// 2**128 = 0o4000000000000000000000000000000000000000000.
	for (const char *end = s + 43; '0' <= *s && *s <= '7' && s < end; ++s) {
		kuint128_imul_u(&t, 8);
		kuint128_iadd_u(&t, *s - '0');
	}
	if (*endptr)
		*endptr = s;
	return t;
}

kuint128 kuint128_init_str(const char *s, const char **endptr) {
	if (s == NULL || !s[0]) {
		if (*endptr)
			*endptr = s;
		return kuint128_init(0, 0);
	}
	while (*s == ' ')
		++s;
	if (*s && memcmp(s, "0x", 2) == 0) {
		return kuint128_init_str_hex(&s[2], endptr);
	} else if (*s && memcmp(s, "0o", 2) == 0) {
		return kuint128_init_str_oct(&s[2], endptr);
	}
	return kuint128_init_str_dec(s, endptr);
}

/* ------------------------------------------------------------------------- */

kuint128 kuint128_lshift(kuint128 t, kuint128 rhs) {
	const uint_fast64_t shift = rhs.LOWER;
	if (rhs.UPPER || (shift >= 128)) {
		return kuint128_0;
	} else if (shift == 64) {
		return kuint128_init(t.LOWER, 0);
	} else if (shift == 0) {
		return t;
	} else if (shift < 64) {
		return kuint128_init((t.UPPER << shift) + (t.LOWER >> (64 - shift)),
				     t.LOWER << shift);
	} else if ((128 > shift) && (shift > 64)) {
		return kuint128_init(t.LOWER << (shift - 64), 0);
	}
	return kuint128_0;
}

kuint128 kuint128_rshift(kuint128 t, kuint128 rhs) {
	const uint_fast64_t shift = rhs.LOWER;
	if (rhs.UPPER || (shift >= 128)) {
		return kuint128_0;
	} else if (shift == 64) {
		return kuint128_init(0, t.UPPER);
	} else if (shift == 0) {
		return t;
	} else if (shift < 64) {
		return kuint128_init(t.UPPER >> shift,
				     (t.UPPER << (64 - shift)) + (t.LOWER >> shift));
	} else if ((128 > shift) && (shift > 64)) {
		return kuint128_init(0, (t.UPPER >> (shift - 64)));
	} else {
		return kuint128_0;
	}
}

kuint128 kuint128_mul(kuint128 t, kuint128 rhs) {
	// split values into 4 32-bit parts
	uint64_t top[4] = {t.UPPER >> 32, t.UPPER & 0xffffffff, t.LOWER >> 32,
			   t.LOWER & 0xffffffff};
	uint64_t bottom[4] = {rhs.UPPER >> 32, rhs.UPPER & 0xffffffff, rhs.LOWER >> 32,
			      rhs.LOWER & 0xffffffff};
	uint64_t products[4][4];

	// multiply each component of the values
	for (int y = 3; y > -1; y--) {
		for (int x = 3; x > -1; x--) {
			products[3 - x][y] = top[x] * bottom[y];
		}
	}

	// first row
	uint64_t fourth32 = (products[0][3] & 0xffffffff);
	uint64_t third32 = (products[0][2] & 0xffffffff) + (products[0][3] >> 32);
	uint64_t second32 = (products[0][1] & 0xffffffff) + (products[0][2] >> 32);
	uint64_t first32 = (products[0][0] & 0xffffffff) + (products[0][1] >> 32);

	// second row
	third32 += (products[1][3] & 0xffffffff);
	second32 += (products[1][2] & 0xffffffff) + (products[1][3] >> 32);
	first32 += (products[1][1] & 0xffffffff) + (products[1][2] >> 32);

	// third row
	second32 += (products[2][3] & 0xffffffff);
	first32 += (products[2][2] & 0xffffffff) + (products[2][3] >> 32);

	// fourth row
	first32 += (products[3][3] & 0xffffffff);

	// move carry to next digit
	third32 += fourth32 >> 32;
	second32 += third32 >> 32;
	first32 += second32 >> 32;

	// remove carry from current digit
	fourth32 &= 0xffffffff;
	third32 &= 0xffffffff;
	second32 &= 0xffffffff;
	first32 &= 0xffffffff;

	// combine components
	return kuint128_init((first32 << 32) | second32, (third32 << 32) | fourth32);
}

static uint8_t *kuint128_ConvertToBuffer(uint8_t *outb, uint_fast64_t val) {
	*outb++ = (val >> 56);
	*outb++ = (val >> 48);
	*outb++ = (val >> 40);
	*outb++ = (val >> 32);
	*outb++ = (val >> 24);
	*outb++ = (val >> 16);
	*outb++ = (val >> 8);
	*outb++ = (val);
	return outb;
}

void kuint128_export_bits(kuint128 t, uint8_t outb[16]) {
	outb = kuint128_ConvertToBuffer(outb, t.UPPER);
	kuint128_ConvertToBuffer(outb, t.LOWER);
}

static uint8_t kuint128_bits(kuint128 t) {
	uint8_t out = 0;
	if (t.UPPER) {
		out = 64;
		uint64_t up = t.UPPER;
		while (up) {
			up >>= 1;
			out++;
		}
	} else {
		uint64_t low = t.LOWER;
		while (low) {
			low >>= 1;
			out++;
		}
	}
	return out;
}

kuint128div kuint128_divmod(kuint128 lhs, kuint128 rhs) {
	// Save some calculations /////////////////////
	assert(kuint128_ne(rhs, kuint128_0));
	if (kuint128_eq(rhs, kuint128_1)) {
		return kuint128div_init(lhs, kuint128_0);
	} else if (kuint128_eq(lhs, rhs)) {
		return kuint128div_init(kuint128_1, kuint128_0);
	} else if (kuint128_eq(lhs, kuint128_0) || kuint128_lt(lhs, rhs)) {
		return kuint128div_init(kuint128_0, lhs);
	}
	kuint128div qr = kuint128div_init(kuint128_0, kuint128_0);
	for (uint8_t x = kuint128_bits(lhs); x > 0; x--) {
		kuint128_ilshift(&qr.qout, kuint128_1);
		kuint128_ilshift(&qr.rem, kuint128_1);

		if (kuint128_bool(kuint128_and(kuint128_rshift_u(lhs, x - 1U), kuint128_1))) {
			kuint128_inc(&qr.rem);
		}

		if (kuint128_ge(qr.rem, rhs)) {
			kuint128_isub(&qr.rem, rhs);
			kuint128_inc(&qr.qout);
		}
	}
	return qr;
}

int kuint128_str(kuint128 t, bool issigned, char *str, size_t size, uint_fast8_t base) {
	if ((base < 2) || (base > 16)) {
		return -EINVAL;
	}
	char buf[KINT128_BIN_STRLEN];
	char *bufend = buf + sizeof(buf);
	char *out = bufend;
	if (kuint128_not(t)) {
		*--out = '0';
	} else {
		const bool negative = issigned && t.UPPER >> 63;
		if (negative) {
			t = kuint128_unsigned_to_signed(t);
		}
		kuint128div qr = kuint128div_init(t, kuint128_0);
		do {
			qr = kuint128_divmod(qr.qout, kuint128_init_u(base));
			const unsigned idx = kuint128_uint8(qr.rem);
			const char c = "0123456789abcdef"[idx];
			*--out = c;
		} while (kuint128_bool(qr.qout));
		if (negative) {
			*--out = '-';
		}
	}
	const size_t len = bufend - out;
	if (size != 0) {
		const size_t sizenz = size - 1;
		const size_t tocopy = len < sizenz ? len : sizenz;
		memcpy(str, out, tocopy);
		str[tocopy] = '\0';
	}
	return len;
}
