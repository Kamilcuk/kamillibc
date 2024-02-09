#ifndef BITPACK_H_
#define BITPACK_H_

#include <stdint.h>
#include <stddef.h>

#define BITPACK_N 8
#include "bitpack_impl.h"

#define BITPACK_N 16
#include "bitpack_impl.h"

#define BITPACK_N 32
#include "bitpack_impl.h"

#define BITPACK_N 64
#include "bitpack_impl.h"

#if defined(__SIZEOF_INT128__) && defined(__GNUC__)
#define uint128_t __uint128_t
#define BITPACK_N 128
#include "bitpack_impl.h"
#endif

#if 0
struct bitpack8 {
	uint8_t *buf;
	size_t buf_size;
	unsigned field_len;
};
void bitpack8_set(struct bitpack8 *t, size_t index, uint8_t value);
uint8_t bitpack8_get(const struct bitpack8 *t, size_t index);
#endif

#endif
