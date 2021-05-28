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
#ifndef KUINT128_H_
#define KUINT128_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __GUNC__
#define _kuint128_const() __attribute__((__const__))
#endif
#ifndef _kuint128_const
#define _kuint128_const()
#endif

typedef struct kuint128 {
	uint_fast64_t LOWER;
	uint_fast64_t UPPER;
} kuint128;

#define KUINT128_INIT(upper, lower)  { lower, upper }

typedef struct kuint128div {
	kuint128 qout;
	kuint128 rem;
} kuint128div;

static const kuint128 kuint128_0 = KUINT128_INIT(0, 0);
static const kuint128 kuint128_1 = KUINT128_INIT(0, 1);
static const kuint128 kuint128_min = KUINT128_INIT(0, 0);
static const kuint128 kuint128_max = KUINT128_INIT(UINT64_MAX, UINT64_MAX);
static const kuint128 kint128_min = KUINT128_INIT(1ull << 63, 0);
static const kuint128 kint128_max = KUINT128_INIT(UINT64_MAX >> 1, UINT64_MAX);

static inline kuint128 kuint128_init(uint_fast64_t upper, uint_fast64_t lower) {
	const kuint128 r = KUINT128_INIT(upper, lower);
	return r;
}

static inline kuint128 kuint128_init_u(uint_fast64_t v) {
	return kuint128_init(0, v);
}

static inline kuint128 kuint128_init_d(int_fast64_t v) {
	return v > 0 ? kuint128_init_u(v) : kuint128_init(UINT64_MAX, v);
}

kuint128 kuint128_init_str_hex(const char *s, const char **endptr);
kuint128 kuint128_init_str_dec(const char *s, const char **endptr);
kuint128 kuint128_init_str_oct(const char *s, const char **endptr);
kuint128 kuint128_init_str(const char *s, const char **endptr);

static inline bool kuint128_bool(kuint128 t) {
    return (bool)(t.UPPER | t.LOWER);
}

static inline uint8_t kuint128_uint8(kuint128 t) {
	return (uint8_t)t.LOWER;
}

static inline uint16_t kuint128_uint16(kuint128 t) {
	return (uint16_t) t.LOWER;
}

static inline uint32_t kuint128_uint32(kuint128 t) {
	return (uint32_t) t.LOWER;
}

static inline uint64_t kuint128_uint64(kuint128 t) {
	return (uint64_t) t.LOWER;
}

static inline kuint128 kuint128_and(kuint128 t, kuint128 rhs) {
    return kuint128_init(t.UPPER & rhs.UPPER, t.LOWER & rhs.LOWER);
}

static inline kuint128 kuint128_iand(kuint128 *t, kuint128 rhs){
	return *t = kuint128_and(*t, rhs);
}

static inline kuint128 kuint128_or(kuint128 t, kuint128 rhs) {
    return kuint128_init(t.UPPER | rhs.UPPER, t.LOWER | rhs.LOWER);
}

static inline kuint128 kuint128_ior(kuint128 *t, kuint128 rhs){
	return *t = kuint128_or(*t, rhs);
}

static inline kuint128 kuint128_xor(kuint128 t, kuint128 rhs) {
    return kuint128_init(t.UPPER ^ rhs.UPPER, t.LOWER ^ rhs.LOWER);
}

static inline kuint128 kuint128_ixor(kuint128 *t, kuint128 rhs){
	return *t = kuint128_xor(*t, rhs);
}

static inline kuint128 kuint128_inv(kuint128 t) {
    return kuint128_init(~t.UPPER, ~t.LOWER);
}

_kuint128_const()
kuint128 kuint128_lshift(kuint128 t, kuint128 rhs);

static inline kuint128 kuint128_ilshift(kuint128 *t, kuint128 rhs) {
	return *t = kuint128_lshift(*t, rhs);
}

_kuint128_const()
kuint128 kuint128_rshift(kuint128 t, kuint128 rhs);

static inline kuint128 kuint128_irshift(kuint128 *t, kuint128 rhs){
	return *t = kuint128_rshift(*t, rhs);
}

static inline bool kuint128_not(kuint128 t) {
    return !kuint128_bool(t);
}

static inline bool kuint128_eq(kuint128 t, kuint128 rhs) {
    return ((t.UPPER == rhs.UPPER) && (t.LOWER == rhs.LOWER));
}

static inline bool kuint128_ne(kuint128 t, kuint128 rhs) {
    return ((t.UPPER != rhs.UPPER) | (t.LOWER != rhs.LOWER));
}

static inline bool kuint128_gt(kuint128 t, kuint128 rhs) {
    if (t.UPPER == rhs.UPPER){
        return (t.LOWER > rhs.LOWER);
    }
    return (t.UPPER > rhs.UPPER);
}

static inline bool kuint128_lt(kuint128 t, kuint128 rhs) {
    if (t.UPPER == rhs.UPPER){
        return (t.LOWER < rhs.LOWER);
    }
    return (t.UPPER < rhs.UPPER);
}

static inline bool kuint128_ge(kuint128 t, kuint128 rhs) {
    return (kuint128_gt(t, rhs) | kuint128_eq(t, rhs));
}

static inline bool kuint128_le(kuint128 t, kuint128 rhs) {
    return (kuint128_lt(t, rhs) | kuint128_eq(t, rhs));
}

static inline kuint128 kuint128_add(kuint128 t, kuint128 rhs) {
    return kuint128_init(t.UPPER + rhs.UPPER + ((t.LOWER + rhs.LOWER) < t.LOWER), t.LOWER + rhs.LOWER);
}

static inline kuint128 kuint128_iadd(kuint128 *t, kuint128 rhs) {
	return *t = kuint128_add(*t, rhs);
}

static inline kuint128 kuint128_sub(kuint128 t, kuint128 rhs) {
    return kuint128_init(t.UPPER - rhs.UPPER - ((t.LOWER - rhs.LOWER) > t.LOWER), t.LOWER - rhs.LOWER);
}

static inline kuint128 kuint128_isub(kuint128 *t, kuint128 rhs) {
	return *t = kuint128_sub(*t, rhs);
}

_kuint128_const()
kuint128 kuint128_mul(kuint128 t, kuint128 rhs);

static inline kuint128 kuint128_imul(kuint128 *t, kuint128 rhs){
	return *t = kuint128_mul(*t, rhs);
}

void kuint128_export_bits(kuint128 t, uint8_t outb[16]);

static inline kuint128div kuint128div_init(kuint128 qout, kuint128 rem) {
	const kuint128div r = { qout, rem };
	return r;
}

_kuint128_const()
kuint128div kuint128_divmod(kuint128 lhs, kuint128 rhs);

static inline kuint128 kuint128_div(kuint128 t, kuint128 rhs) {
    return kuint128_divmod(t, rhs).qout;
}

static inline kuint128 kuint128_idiv(kuint128 *t, kuint128 rhs){
	return *t = kuint128_div(*t, rhs);
}

static inline kuint128 kuint128_mod(kuint128 t, kuint128 rhs) {
    return kuint128_divmod(t, rhs).rem;
}

static inline kuint128 kuint128_imod(kuint128 *t, kuint128 rhs){
	return *t = kuint128_mod(*t, rhs);
}

static inline void kuint128_inc(kuint128 *t) {
	kuint128_iadd(t, kuint128_1);
}

static inline kuint128 kuint128_postinc(kuint128 *t) {
	const kuint128 temp = *t;
	kuint128_inc(t);
	return temp;
}

static inline void kuint128_dec(kuint128 *t){
	kuint128_isub(t, kuint128_1);
}

static inline kuint128 kuint128_postdec(kuint128 *t) {
    kuint128 temp = *t;
	kuint128_dec(t);
	return temp;
}

static inline kuint128 kuint128_neg(kuint128 t) {
	return kuint128_add(kuint128_neg(t), kuint128_1);
}

static inline uint_fast64_t kuint128_upper(kuint128 t) {
    return t.UPPER;
}

static inline uint_fast64_t kuint128_lower(kuint128 t) {
    return t.LOWER;
}

static inline kuint128 kuint128_unsigned_to_signed(kuint128 t) {
	return kuint128_add(kuint128_inv(t), kuint128_1);
}

static inline kuint128 kuint128_signed_to_unsigned(kuint128 t) {
	return kuint128_inv(kuint128_sub(t, kuint128_1));
}

/* to string conversions ------------------------------------------------------------------------- */

/**
 * Converts a kuint128 variable into a string.
 * Writes up to size bytes into the memory pointed to by str.
 * Base is the conversion base, may be equal from 2 to 16.
 * @returns -EINVAL on success, otherwise the count of bytes that
 *          __would have been written__ had the size would been big enough
 *          (like snprintf).
 */
int kuint128_str(kuint128 t, bool issigned, char *str, size_t size, uint_fast8_t base);

// Number of bytes needed to store 2^128 in base 2.
#define KUINT128_BIN_STRLEN   257
// Number of bytes needed to store 2^128 in base 8.
// ln(2^128)/ln(8) = 42. echo 'obase=8; 2^128' | bc | wc - 44
#define KUINT128_OCT_STRLEN   44
// Number of bytes needed to store 2^128 in base 10.
// lopg10(2^128) = 39 . echo 2^128 | bc | wc -c -> 40
#define KUINT128_DEC_STRLEN  40
// Number of bytes needed to store 2^128 in base 16.
#define KUINT128_HEX_STRLEN  33

#define KUINT128_STRLEN      KUINT128_DEC_STRLEN

// Same, but for signed kuint128.
#define KINT128_BIN_STRLEN   (1+KUINT128_BIN_STRLEN)
#define KINT128_OCT_STRLEN   45
#define KINT128_DEC_STRLEN   41
#define KINT128_HEX_STRLEN   34
#define KINT128_STRLEN       KINT128_DEC_STRLEN

static inline char *_kuint128_str_in(kuint128 t, bool issigned, char *str, size_t size, uint_fast8_t base) {
	kuint128_str(t, issigned, str, size, base);
	return str;
}
#define _kuint128_str(t, i, buf, base)  _kuint128_str_in(t, i, buf, sizeof(buf), base)
// Converts a kuint128 variable into a string in various bases.
#define KUINT128_BINSTR(t)  _kuint128_str(t, 0, (char[KUINT128_BIN_STRLEN]){0}, 2)
#define KUINT128_OCTSTR(t)  _kuint128_str(t, 0, (char[KUINT128_OCT_STRLEN]){0}, 8)
#define KUINT128_DECSTR(t)  _kuint128_str(t, 0, (char[KUINT128_DEC_STRLEN]){0}, 10)
#define KUINT128_HEXSTR(t)  _kuint128_str(t, 0, (char[KUINT128_HEX_STRLEN]){0}, 16)
#define KUINT128_STR         KUINT128_DECSTR
// Converts a signed kuint128 variable into a string in various bases.
#define KINT128_BINSTR(t)   _kuint128_str(t, 1, (char[KINT128_BIN_STRLEN]){0}, 2)
#define KINT128_OCTSTR(t)   _kuint128_str(t, 1, (char[KINT128_OCT_STRLEN]){0}, 8)
#define KINT128_DECSTR(t)   _kuint128_str(t, 1, (char[KINT128_DEC_STRLEN]){0}, 10)
#define KINT128_HEXSTR(t)   _kuint128_str(t, 1, (char[KINT128_HEX_STRLEN]){0}, 16)
#define KINT128_STR         KINT128_DECSTR

/* _u suffix functions ------------------------------------------------------------------------- */

#define _uint128_gen_b(OP) \
static inline bool kuint128_##OP##_u(kuint128 t, uint_fast64_t o) { \
	return kuint128_##OP(t, kuint128_init_u(o)); \
} \
static inline bool kuint128_##OP##_d(kuint128 t, int_fast64_t o) { \
	return kuint128_##OP(t, kuint128_init_d(o)); \
}
#define _uint128_gen_u(OP) \
static inline kuint128 kuint128_##OP##_d(kuint128 t, int_fast64_t o) { \
	return kuint128_##OP(t, kuint128_init_d(o)); \
} \
static inline kuint128 kuint128_i##OP##_d(kuint128 *t, int_fast64_t o) { \
	return kuint128_i##OP(t, kuint128_init_d(o)); \
} \
static inline kuint128 kuint128_##OP##_u(kuint128 t, uint_fast64_t o) { \
	return kuint128_##OP(t, kuint128_init_u(o)); \
} \
static inline kuint128 kuint128_i##OP##_u(kuint128 *t, uint_fast64_t o) { \
	return kuint128_i##OP(t, kuint128_init_u(o)); \
}

_uint128_gen_u(add)
_uint128_gen_u(sub)
_uint128_gen_u(mul)
_uint128_gen_u(div)
_uint128_gen_u(mod)
_uint128_gen_u(rshift)
_uint128_gen_u(lshift)
_uint128_gen_u(and)
_uint128_gen_u(or)
_uint128_gen_u(xor)
_uint128_gen_b(eq)
_uint128_gen_b(ne)
_uint128_gen_b(lt)
_uint128_gen_b(le)
_uint128_gen_b(gt)
_uint128_gen_b(ge)

#undef _uint128_gen_u
#undef _uint128_gen_b

/* ------------------------------------------------------------------------- */

// #define UINT128_DEBUGGING 1
#ifdef UINT128_DEBUGGING
#include <stdio.h>
#include <inttypes.h>
static inline void kuint128_println(kuint128 t) {
	printf("(%llu,%llu)\n", (unsigned long long)t.UPPER, (unsigned long long)t.LOWER);
}
#else
static inline void kuint128_println(kuint128 t) {}
#endif

/* _Generic ------------------------------------------------------------------------- */

static inline kuint128 _kuint128_g_str(const char *str) {
	return kuint128_init_str(str, NULL);
}
static inline kuint128 _kuint128_g_identity(kuint128 x) {
	return x;
}
#define _kuint128_init_g_1(x) _Generic((x), \
		const char *: _kuint128_g_str, \
		char *: _kuint128_g_str, \
		kuint128: _kuint128_g_identity, \
		const kuint128: _kuint128_g_identity, \
		default: kuint128_init_u)(x)
#define _kuint128_init_g_2(x,y) _Generic((x), \
		const char *: kuint128_init_str, \
		char *: kuint128_init_str, \
		default: kuint128_init)(x, y)
#define _kuint128_init_g_N(_1,_2,N,...) _kuint128_init_g_##N
#define kuint128_n(...) _kuint128_init_g_N(__VA_ARGS__,2,1)(__VA_ARGS__)

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif

