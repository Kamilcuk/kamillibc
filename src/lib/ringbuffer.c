/*
 * ringthisfer.c
 *
 *  Created on: 28.01.2018
 *      Author: kamil
 */
#include "ringbuffer.h"

#include <minmax.h>
#include <uni/cdefs.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/* Private Functions ------------------------------------------------------- */

static inline __pure2 size_t LIMIT(size_t val, size_t size)
{
#if RB_SIZE_MUST_BE_A_POWER_OF_2
	assert(RB_ISPOWEROF2(size));
	return val & (size - 1);
#else
	return val % size;
#endif
}

static inline void rb_advance_tail(RingBuffer_t *this, size_t bytes)
{
	this->fill += bytes;
}

static inline void rb_advance_head(RingBuffer_t *this, size_t bytes)
{
	this->head = LIMIT(this->head + bytes, this->size);
	this->fill -= bytes;
}

static inline size_t rb_get_tail(RingBuffer_t *this, size_t offset)
{
	return LIMIT(this->head + this->fill + offset, this->size);
}

/* Public Functions ------------------------------------------------------- */

RingBuffer_t *rb_new(size_t size)
{
	RingBuffer_t * const this = malloc(sizeof(*this));
	if (this == NULL) goto ERROR_THIS_MALLOC;

	char * const data = malloc(size);
	if(data == NULL) goto ERROR_DATA_MALLOC;

	rb_init(this, data, size);

	return this;
	ERROR_DATA_MALLOC:
	free(this);
	ERROR_THIS_MALLOC:
	return NULL;
}

void rb_free(RingBuffer_t **this)
{
	assert(this != NULL);
	if(*this != NULL) {
		free((*this)->data);
		free(*this);
		*this = NULL;
	}
}

void rb_init(RingBuffer_t *this, char data[], size_t size)
{
	assert(this != NULL);
	assert(RB_ISPOWEROF2(size));
	assert(data != NULL);
	this->data = data;
	this->size = size;
	this->head = this->fill = 0;
}

void rb_write(RingBuffer_t *this, const char from[], size_t bytes)
{
 	rb_write_memcpy(this, from, bytes);

	rb_write_commit(this, bytes);
}

char *rb_write_pointer(RingBuffer_t *this, size_t *writable)
{
	assert(writable != NULL);

	if(rb_is_full(this)) {
		return NULL;
	}

	const size_t head = this->head;
	const size_t tail = rb_get_tail(this, 0);
	const size_t end = this->size;

	*writable = tail < head ? head - tail : end - tail;

	return &this->data[tail];
}

void rb_write_memcpy(RingBuffer_t *this, const char from[], size_t bytes)
{
	assert(bytes <= rb_remain(this));

	const size_t tail = rb_get_tail(this, 0);
	const size_t end = this->size;
	const size_t write_end = tail + bytes;

	if (write_end <= end) {
		memcpy(&this->data[tail], from, bytes);
	} else {
		const size_t first_write = end - tail;
		memcpy(&this->data[tail], from, first_write);

		const size_t second_write = bytes - first_write;
		memcpy(&this->data[0], &from[first_write], second_write);
	}
}

void rb_write_commit(RingBuffer_t *this, size_t bytes)
{
	assert(bytes <= rb_remain(this));
	rb_advance_tail(this, bytes);
}

void rb_read(RingBuffer_t *this, char to[], size_t bytes)
{
	rb_read_memcpy(this, to, bytes);

	rb_read_commit(this, bytes);
}

const char *rb_read_pointer(RingBuffer_t *this, size_t offset, size_t *readable)
{
	assert(readable != NULL);

	if(rb_is_empty(this)) {
		return NULL;
	}

	const size_t head = this->head + offset;
	const size_t tail = rb_get_tail(this, offset);
	const size_t end = this->size;

	*readable = tail <= head ? end - head : tail - head;

	return &this->data[head];
}

void rb_read_memcpy(RingBuffer_t *this, char to[], size_t bytes)
{
	assert(this != NULL);
	assert(rb_used(this) >= bytes);

	const size_t head = this->head;
	const size_t end = this->size;
	const size_t read_end = LIMIT(head + bytes, this->size);

	if(read_end < head) {
		const size_t first_read = end - head;
		memcpy(&to[0], &this->data[head], first_read);

		const size_t second_read = bytes - first_read;
		memcpy(&to[first_read], &this->data[0], second_read);
	} else {
		memcpy(&to[0], &this->data[head], bytes);
	}
}

void rb_read_commit(RingBuffer_t *this, size_t bytes)
{
	assert(rb_used(this) >= bytes);
	rb_advance_head(this, bytes);
}

void rb_stream(RingBuffer_t *from, RingBuffer_t *to, size_t bytes)
{
	assert(from != to);
	assert(rb_used(from) <= bytes);
	assert(rb_remain(to) >= bytes);

	size_t copied = 0;
	while(copied < bytes) {
		size_t can_read;
		const char * const from_ptr = rb_read_pointer(from, copied, &can_read);

		size_t copied_rb_read = 0;

		while(copied_rb_read < can_read) {
			size_t can_write;
			char * to_ptr = rb_write_pointer(to, &can_write);

			size_t write = (can_read > can_write) ? can_write : can_read;
			memcpy(to_ptr, from_ptr, write);

			copied_rb_read += write;
		}

		copied += copied_rb_read;
	}

	rb_advance_tail(to, copied);
}

int rb_unittest()
{
	RingBuffer_t rb_mem = RB_INIT_ONSTACK(16);
	RingBuffer_t * const rb = &rb_mem;
	char data[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
	char tmp[sizeof(data)/sizeof(data[0])] = {0};
	size_t datalen = MIN(sizeof(data)/sizeof(data[0]), rb->size) - 1;

	for(int i=0;i<2048;++i) {

		for(size_t nbyte = 0; nbyte < datalen; ++nbyte) {
			rb_write(rb, data, nbyte);
			rb_read(rb, tmp, nbyte);
			if(memcmp(data, tmp, nbyte)) { return -__LINE__; }
		}

		for(size_t nbyte = 0; nbyte < datalen; ++nbyte) {
			size_t writesize;
			char * const writepnt = rb_write_pointer(rb, &writesize);
			size_t nbyte2 = MIN(writesize, nbyte);
			memcpy(writepnt, data, nbyte2);
			rb_write_commit(rb, nbyte2);

			rb_read(rb, tmp, nbyte2);
			if(memcmp(data, tmp, nbyte2)) { return -__LINE__; }
		}

		for(size_t nbyte = 0; nbyte < datalen; ++nbyte) {
			size_t writesize;
			char * const writepnt = rb_write_pointer(rb, &writesize);
			size_t nbyte2 = MIN(writesize, nbyte);
			memcpy(writepnt, data, nbyte2);
			rb_write_commit(rb, nbyte2);

			size_t readsize;
			const char * const readpnt = rb_read_pointer(rb, 0, &readsize);
			size_t nbyte3 = MIN(readsize, nbyte2);
			memcpy(tmp, readpnt, nbyte3);
			if(memcmp(data, tmp, nbyte3)) { return -__LINE__; }
			rb_read_commit(rb, nbyte3);

			if(!rb_is_empty(rb)) { return -__LINE__; }
		}

		for(size_t nbyte = 0; nbyte < datalen; ++nbyte) {
			for(size_t i = nbyte; i; ) {
				size_t writesize;
				char * const writepnt = rb_write_pointer(rb, &writesize);
				size_t nbyte2 = MIN(writesize, i);
				memcpy(writepnt, data, nbyte2);
				rb_write_commit(rb, nbyte2);
				i-=nbyte2;
			}
			for(size_t i = nbyte; i; ) {
				size_t readsize;
				const char * const readpnt = rb_read_pointer(rb, 0, &readsize);
				size_t nbyte3 = MIN(readsize, i);
				memcpy(tmp, readpnt, nbyte3);
				if(memcmp(data, tmp, nbyte3)) { return -__LINE__; }
				rb_read_commit(rb, nbyte3);
				i-=nbyte3;
			}

			if(!rb_is_empty(rb)) { return -__LINE__; }
		}
	}
	return 0;
}
