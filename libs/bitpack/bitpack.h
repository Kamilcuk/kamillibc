#ifndef BITPACK_H_
#define BITPACK_H_

#include <stdint.h>
#include <stddef.h>

struct bitpack8 {
	uint8_t *buf;
	size_t buf_size;
	unsigned field_len;
};
void bitpack8_set(struct bitpack8 *t, size_t index, uint8_t value);
uint8_t bitpack8_get(const struct bitpack8 *t, size_t index);

struct bitpack16 {
	uint16_t *buf;
	size_t buf_size;
	unsigned field_len;
};
void bitpack16_set(struct bitpack16 *t, size_t index, uint16_t value);
uint16_t bitpack16_get(const struct bitpack16 *t, size_t index);

struct bitpack32 {
	uint32_t *buf;
	size_t buf_size;
	unsigned field_len;
};
void bitpack32_set(struct bitpack32 *t, size_t index, uint32_t value);
uint32_t bitpack32_get(const struct bitpack32 *t, size_t index);


#endif
