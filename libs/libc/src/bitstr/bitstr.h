/*
 * bitstr.h
 *
 *  Created on: 22 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_BITSTR_BITSTR_H_
#define SRC_LIB_BITSTR_BITSTR_H_

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

struct bitstr_s {
	char * const buf;
	const size_t size;
	size_t pos;
	unsigned char bitpos;
};

#define BITSTR_INIT(_buf,_size)  { .buf = (_buf), .size = (_size) }

struct bitstr_s bitstr_init(char buf[], size_t size);

void bitstr_incbitpos(struct bitstr_s *t);
void bitstr_decbitpos(struct bitstr_s *t);

char bitstr_cur_bitmask(struct bitstr_s *t);

bool bitstr_get(struct bitstr_s *t);
bool bitstr_getinc(struct bitstr_s *t);
bool bitstr_getdec(struct bitstr_s *t);
void bitstr_put(struct bitstr_s *t, bool val);
void bitstr_putinc(struct bitstr_s *t, bool val);
void bitstr_putdec(struct bitstr_s *t, bool val);

static inline void bitstr_begin(struct bitstr_s *t)
{
	assert(t != NULL);
	t->bitpos = 0;
	t->pos = 0;
}

static inline bool bitstr_atBegin(struct bitstr_s *t)
{
	assert(t != NULL);
	return t->pos == 0 && t->bitpos == 0;
}

static inline void bitstr_end(struct bitstr_s *t)
{
	assert(t != NULL);
	t->bitpos = 0;
	t->pos = t->size;
}

static inline bool bitstr_atEnd(struct bitstr_s *t)
{
	assert(t != NULL);
	return t->pos == t->size;
}

int bitstr_unittest(void);

#endif /* SRC_LIB_BITSTR_BITSTR_H_ */
