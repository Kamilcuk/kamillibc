/*
 * ringbuffer.h
 *
 *  Created on: 28.01.2018
 *      Author: kamil
 */

#ifndef RIRC_RING_BUFFER_H
#define RIRC_RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

/* Private Macros ----------------------------------------------------------- */

#ifndef RB_SIZE_MUST_BE_A_POWER_OF_2
#define RB_SIZE_MUST_BE_A_POWER_OF_2  1
#endif

#if RB_SIZE_MUST_BE_A_POWER_OF_2
#define RB_ISPOWEROF2(x)  (!( (((x) < 2) || ((x) & ((x) - 1))) ))
#else
#define RB_ISPOWEROF2(x)  (1)
#endif

/* Exported Types ------------------------------------------------------------ */

typedef struct {
	char *data;
    size_t size;
    size_t head;
    size_t fill;
} RingBuffer_t;

/* Exported Functions --------------------------------------------------------- */

#define RB_INIT(_data, _size)   { \
	.data = sizeof(char[RB_ISPOWEROF2(_size)?1:-1]) ? (_data) : NULL, \
	.size = (_size) \
	}
#define RB_INIT_ONSTACK(_size)  RB_INIT( ((char[(_size)]){0}), (_size) )

RingBuffer_t *rb_new(size_t size);
void rb_free(RingBuffer_t **this);

void rb_init(RingBuffer_t *this, char data[], size_t size);

void rb_write(RingBuffer_t *this, const char from[], size_t bytes);
char *rb_write_pointer(RingBuffer_t *this, size_t *writable);
void rb_write_memcpy(RingBuffer_t *this, const char from[], size_t bytes);
void rb_write_commit(RingBuffer_t *this, size_t bytes);

void rb_read(RingBuffer_t *this, char to[], size_t bytes);
const char *rb_read_pointer(RingBuffer_t *this, size_t offset, size_t *readable);
void rb_read_memcpy(RingBuffer_t *this, char to[], size_t bytes);
void rb_read_commit(RingBuffer_t *this, size_t bytes);

void rb_stream(RingBuffer_t *from, RingBuffer_t *to, size_t bytes);

int rb_unittest();

static inline void rb_flush(RingBuffer_t *this)
{
	assert(this != NULL);
	this->head = this->fill = 0;
}

static inline bool rb_is_empty(RingBuffer_t *this)
{
	assert(this != NULL);
    return this->fill == 0;
}

static inline bool rb_is_full(RingBuffer_t *this)
{
	assert(this != NULL);
    return this->fill == this->size;
}

static inline size_t rb_size(RingBuffer_t *this)
{
	assert(this != NULL);
    return this->size;
}

static inline size_t rb_used(RingBuffer_t *this)
{
	assert(this != NULL);
    return this->fill;
}

static inline size_t rb_remain(RingBuffer_t *this)
{
	assert(this != NULL);
    return this->size - this->fill;
}

#endif
