#ifndef BITPACK_IMPL_H_
#define BITPACK_IMPL_H_
#include "bitpack.h"

#ifndef BITPACK_DEBUG
#define BITPACK_DEBUG 1
#endif

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#if BITPACK_DEBUG
#include <stdio.h>
#endif

#define BITPACK_CONCAT(a, b, c, d) a##b##c##d
#define BITPACK_XCONCAT(a, b, c, d) BITPACK_CONCAT(a, b, c, d)
#define BITPACK_SYM(suff) BITPACK_XCONCAT(bitpack, BITPACK_N, _, suff)

enum bitpack_op {
	bitpack_set,
	bitpack_get,
	bitpack_size,
};

#endif // BITPACK_IMPL_H_

#ifndef BITPACK_N
#error BITPACK_N is missing
#define BITPACK_N 8
#endif

#define BITPACK_T BITPACK_XCONCAT(uint, BITPACK_N, _, t)
#define BITPACK_S struct BITPACK_XCONCAT(bitpack, BITPACK_N, , )

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
static inline BITPACK_T BITPACK_SYM(do)(const BITPACK_S *t, size_t index, enum bitpack_op op, BITPACK_T value) {
	BITPACK_T *buf = t->buf;
	const size_t buf_size = t->buf_size;
	const unsigned field_len = t->field_len;
	//
	value &= (1u << field_len) - 1u;
	const size_t type_bit = CHAR_BIT * sizeof(BITPACK_T);
	const BITPACK_T type_max = ~(BITPACK_T)0;
	const size_t buf_bits = buf_size * type_bit;
	const size_t start = index * field_len; // start bit index
	BITPACK_T stop = (index + 1) * field_len; // stop bit index
	if (stop > buf_bits)
		stop = buf_bits; // limit stop bit index to the size of the buffer.
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
	const bool use_stopbyte = stopbyte != startbyte && stopbit != 0;
	//
	switch (op) {
	case bitpack_set: {
#if BITPACK_DEBUG
		// printf("type_bit=%zu type_max=%x buf_bits=%zu\n", type_bit, type_max, buf_bits);
		printf("(buf[%zu] & %x) | (%x >> %u) -> %x | %x -> %x\n", startbyte, startclear, value, startbit,
		       buf[startbyte] & startclear, (BITPACK_T)(value << startbit),
		       (buf[startbyte] & startclear) | (BITPACK_T)(value << startbit));
		if (use_stopbyte) {
			printf("(buf[%zu] & %x) | (%x << %u) -> %x | %x -> %x\n", stopbyte, stopclear, value,
			       startbitrev, buf[stopbyte] & stopclear, value >> startbitrev,
			       (buf[stopbyte] & stopclear) | (BITPACK_T)(value >> startbitrev));
		}
#endif
		buf[startbyte] = (buf[startbyte] & startclear) | (BITPACK_T)(value << startbit);
		if (use_stopbyte) {
			buf[stopbyte] = (buf[stopbyte] & stopclear) | (value >> startbitrev);
		}
	} break;
	case bitpack_get: {
		const BITPACK_T l = (buf[startbyte] >> startbit) & ~startclear;
		const BITPACK_T r = !use_stopbyte ? 0 : (buf[stopbyte] << startbitrev) & ~stopclear;
		const BITPACK_T ret = l | r;
#if BITPACK_DEBUG
		printf("((buf[%zu] & %x) >> %u) | ((buf[%zu] & %x) << %u) -> %x | %x -> %x\n", startbyte, ~startclear,
		       startbit, stopbyte, ~stopclear, stopbitrev, l, r, ret);
#endif
		return ret;
	} break;
	case bitpack_size:
		return buf_bits / field_len + !!(buf_bits % field_len);
	default:
		assert(0); // invalid op argument
		break;
	}
	return 0;
}

void BITPACK_SYM(set)(BITPACK_S *t, size_t index, BITPACK_T value) {
	BITPACK_SYM(do)(t, index, bitpack_set, value);
}

BITPACK_T BITPACK_SYM(get)(const BITPACK_S *t, size_t index) {
	return BITPACK_SYM(do)(t, index, bitpack_get, 0);
}

size_t BITPACK_SYM(size)(const BITPACK_S *t) {
	return BITPACK_SYM(do)(t, 0, bitpack_size, 0);
}

#ifdef BITPACK_T
#undef BITPACK_T
#endif
#ifdef BITPACK_S
#undef BITPACK_S
#endif
#ifdef BITPACK_N
#undef BITPACK_N
#endif
