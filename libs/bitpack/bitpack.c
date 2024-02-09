#define BITPACK_DEBUG 1
#define BITPACK_TEST 1

#include "bitpack.h"

#if BITPACK_TEST

#include <assert.h>
#include <stdio.h>

static inline unsigned char bitreverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

static inline void buf_println(size_t size, void *v, unsigned sep) {
	const unsigned nl = 13 * CHAR_BIT;
	unsigned char *c = v;
	for (unsigned i = 0; i < size * CHAR_BIT; ++i) {
		unsigned char bit = !!(c[i / CHAR_BIT] & (1 << (i % CHAR_BIT)));
		if (i % nl == nl - 1) {
			printf("\n");
		} else if (i > 0 && (i - 1) % sep == sep - 1) {
			printf("'");
		} else if (i > 0 && (i - 1) % 8 == 7) {
			printf("|");
		}
		printf("%d", bit);
	}
	printf("\n");
}

int main() {
	if (1) {
		uint8_t buf[5] = {0};
		struct bitpack8 *t = &(struct bitpack8){buf, sizeof(buf)/sizeof(buf[0]), 7};
		for (size_t i = 0; i < bitpack8_size(t); ++i) {
			bitpack8_set(t, i, 0xff);
			buf_println(t->buf_size, t->buf, 7);
		}
		bitpack8_set(t, 3, 0);
		buf_println(t->buf_size, t->buf, 7);
	}
	if (1) {
		for (int move = 0; move < 15; move++) {
			for (int bit = 0; bit < move; ++bit) {
				int val = 1 << bit;
				uint16_t buf[5] = {0};
				struct bitpack16 *t = &(struct bitpack16){buf, sizeof(buf)/sizeof(buf[0]), move};
				for (size_t i = 0; i < bitpack16_size(t); ++i) {
					bitpack16_set(t, i, val);
					buf_println(sizeof(buf), buf, move);
				}
			}
		}
	}
}

#endif
