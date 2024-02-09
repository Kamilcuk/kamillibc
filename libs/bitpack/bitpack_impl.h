#ifndef BITPACK_IMPL_H_
#define BITPACK_IMPL_H_

#ifndef BITPACK_DEBUG
#define BITPACK_DEBUG 1
#endif
#ifndef BITPACK_TEST
#define BITPACK_TEST 1
#endif

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if BITPACK_DEBUG
#include <stdio.h>
#endif
#if BITPACK_TEST
#include <assert.h>
#endif

#if BITPACK_DEBUG
#define BITPACK_DEBUGF(fmt, ...) fprintf(stderr, "%d:" fmt, BITPACK_N, __VA_ARGS__)
#else
#define BITPACK_DEBUGF(fmt, ...)
#endif

#define BITPACK_CONCAT(a, b, c, d) a##b##c##d
#define BITPACK_XCONCAT(a, b, c, d) BITPACK_CONCAT(a, b, c, d)
#define BITPACK_DO(suff) BITPACK_XCONCAT(bitpack, BITPACK_N, _, suff)
#define BITPACK_T BITPACK_XCONCAT(uint, BITPACK_N, _, t)
#define BITPACK_S struct BITPACK_XCONCAT(bitpack, BITPACK_N, , )

enum _bitpack_op {
	_bitpack_set,
	_bitpack_get,
	_bitpack_size,
};

#endif // BITPACK_IMPL_H_

/* ------------------------------------------------------------------------- */

#ifndef BITPACK_N
#error BITPACK_N is missing
#define BITPACK_N 8
#endif

BITPACK_S {
	BITPACK_T *buf;
	size_t buf_size;
	unsigned field_len;
};

/**
 * Do an operation on an array of tighly packed bitfields.
 *
 * @param buf an array with bits
 * @param field_len The length of the bitfield.
 * @param field_cnt The count of bitfields.
 * @param index The index of bitfield to operate on.
 * @param op Set or get the bitfield.
 * @param value Value to set or ignored.
 */
static inline BITPACK_T BITPACK_DO(do)(const BITPACK_S *t, size_t index, enum _bitpack_op op, BITPACK_T value) {
	BITPACK_T *buf = t->buf;
	const size_t buf_size = t->buf_size;
	const unsigned field_len = t->field_len;
	//
	value &= (1u << field_len) - 1u;
	const size_t type_bit = CHAR_BIT * sizeof(BITPACK_T);
	const BITPACK_T type_max = ~(BITPACK_T)0;
	const size_t buf_bits = buf_size * type_bit;
	const size_t start = index * field_len; // start bit index
	const size_t stop = (index + 1) * field_len; // stop bit index
	const size_t startbyte = start / type_bit; // starting byte index
	const unsigned char startbit = start % type_bit; // start bit index in starting byte
	const unsigned char startbitrev = type_bit - startbit; // start bit index in starting byte
	const BITPACK_T startclear = (1u << startbit) - 1; // clear starting byte
	const size_t stopbyte = stop / type_bit; // stopping byte index
	const unsigned char stopbit = stop % type_bit; // stop bit index in stopping byte
	const unsigned char stopbitrev = type_bit - 1 - stopbit; // stop bit index indexed reversed
	const BITPACK_T stopclear = type_max << stopbit; // clear stopping byte
	assert(field_len < type_bit); // bitfield length has to be lower than bits in the type
	assert(start < buf_size * type_bit); // the index has to be lower than the buffer size
	const bool use_stopbyte = stopbyte < buf_size && stopbyte != startbyte && stopbit != 0;
	//
	switch (op) {
	case _bitpack_set: {
		// printf("type_bit=%zu type_max=%x buf_bits=%zu\n", type_bit, type_max, buf_bits);
		BITPACK_DEBUGF("set1 (buf[%zu] & %x) | (%x >> %u) -> %x | %x -> %x\n", startbyte, startclear, value,
			       startbit, buf[startbyte] & startclear, (BITPACK_T)(value << startbit),
			       (buf[startbyte] & startclear) | (BITPACK_T)(value << startbit));
		buf[startbyte] = (buf[startbyte] & startclear) | (BITPACK_T)(value << startbit);
		if (use_stopbyte) {
			BITPACK_DEBUGF("set2 (buf[%zu] & %x) | (%x << %u) -> %x | %x -> %x\n", stopbyte, stopclear,
				       value, startbitrev, buf[stopbyte] & stopclear, value >> startbitrev,
				       (buf[stopbyte] & stopclear) | (BITPACK_T)(value >> startbitrev));
			buf[stopbyte] = (buf[stopbyte] & stopclear) | (value >> startbitrev);
		}
#if BITPACK_TEST
		{
			const BITPACK_T result = BITPACK_DO(do)(t, index, _bitpack_get, 0);
			const unsigned endmaskmove = field_len - (stop - buf_bits);
			const unsigned endmask = stop > buf_bits ? (1 << endmaskmove) - 1 : type_max;
			BITPACK_DEBUGF("cmp get(%zu) ?= (%x & %x) -> %x ?= %x | %zu %zu %u\n", index, value, endmask, result,
				       value & endmask, stop, buf_bits, endmaskmove);
			assert(result == (value & endmask));
		}
#endif
	} break;
	case _bitpack_get: {
		const BITPACK_T l = (buf[startbyte] & ~startclear) >> startbit;
		const BITPACK_T r = !use_stopbyte ? 0 : (buf[stopbyte] & ~stopclear) << startbitrev;
		const BITPACK_T ret = l | r;
		BITPACK_DEBUGF("get ((buf[%zu] & %x) >> %u) | ((buf[%zu] & %x) << %u) -> %x | %x -> %x\n", startbyte,
			       ~startclear, startbit, stopbyte, ~stopclear, stopbitrev, l, r, ret);
		return ret;
	} break;
	case _bitpack_size:
		return buf_bits / field_len + !!(buf_bits % field_len);
	default:
		assert(0); // invalid op argument
		break;
	}
	return 0;
}

static inline BITPACK_T BITPACK_DO(get)(const BITPACK_S *t, size_t index) {
	return BITPACK_DO(do)(t, index, _bitpack_get, 0);
}

static inline void BITPACK_DO(set)(BITPACK_S *t, size_t index, BITPACK_T value) {
	BITPACK_DO(do)(t, index, _bitpack_set, value);
}

static inline size_t BITPACK_DO(size)(const BITPACK_S *t) {
	return BITPACK_DO(do)(t, 0, _bitpack_size, 0);
}

#ifdef BITPACK_N
#undef BITPACK_N
#endif
