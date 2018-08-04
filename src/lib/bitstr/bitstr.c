/*
 * bitstr.c
 *
 *  Created on: 22 kwi 2018
 *      Author: kamil
 */

#include <bitstr/bitstr.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

struct bitstr_s bitstr_init(char buf[], size_t size) {
	assert(buf != NULL);
	(void)memset(buf, 0, size);
	struct bitstr_s ret = BITSTR_INIT(buf,size);
	return ret;
}

void bitstr_incbitpos(struct bitstr_s *t)
{
	assert(t);
	assert(t->pos < t->size || (t->pos == t->size && t->bitpos == 0));
	++t->bitpos;
	if (t->bitpos == CHAR_BIT) {
		t->bitpos = 0;
		++t->pos;
	}
}

void bitstr_decbitpos(struct bitstr_s *t)
{
	assert(t);
	if (t->bitpos != 0) {
		--t->bitpos;
	} else {
		t->bitpos = CHAR_BIT;
		assert(t->pos > 0);
		--t->pos;
	}
}

char bitstr_cur_bitmask(struct bitstr_s *t)
{
	assert(t != NULL);
	assert(0 <= t->bitpos);
	assert(t->bitpos < CHAR_BIT);
	return 1 << t->bitpos;
}

bool bitstr_get(struct bitstr_s *t)
{
	return bitstr_cur_bitmask(t) & t->buf[t->pos];
}

bool bitstr_getinc(struct bitstr_s *t)
{
	const bool ret = bitstr_get(t);
	bitstr_incbitpos(t);
	return ret;
}

bool bitstr_getdec(struct bitstr_s *t)
{
	const bool ret = bitstr_get(t);
	bitstr_decbitpos(t);
	return ret;
}

void bitstr_put(struct bitstr_s *t, bool val)
{
	const char bitmask = bitstr_cur_bitmask(t);
	if (val) {
		t->buf[t->pos] |= bitmask;
	} else {
		assert((t->buf[t->pos] & bitmask) == 0);
		t->buf[t->pos] &= ~bitmask;
	}
}

void bitstr_putinc(struct bitstr_s *t, bool val)
{
	bitstr_put(t, val);
	bitstr_incbitpos(t);
}

void bitstr_putdec(struct bitstr_s *t, bool val)
{
	bitstr_put(t, val);
	bitstr_decbitpos(t);
}

int bitstr_unittest(void)
{
#define TEST(expr) if(!(expr)) { assert(expr); return -__LINE__; }
	struct bitstr_s t = BITSTR_INIT(((char*)(unsigned char[2]){0xAA, 0xAA}), 2);
	TEST(bitstr_atBegin(&t) == true);
	for(size_t i = 0; i < CHAR_BIT; ++i) {
		TEST(bitstr_getinc(&t) == false);
		TEST(bitstr_getinc(&t) == true);
	}
	TEST(bitstr_atEnd(&t) == true);
	return 0;
}
