/*
 * qiter.h
 *
 *  Created on: 30 sty 2020
 *      Author: kamil
 */

#ifndef SRC_QITER_QITER_H_
#define SRC_QITER_QITER_H_

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
typedef int errno_t;

typedef struct qiter_s qiter_t;

struct qiter_vtable_s {
	errno_t (*copy)(qiter_t *t, qiter_t *o);
	errno_t (*seek)(qiter_t *t, ptrdiff_t seek);
	errno_t (*tell)(qiter_t *t, size_t *pos);
	errno_t (*size)(qiter_t *t, size_t *size);
	errno_t (*el_get)(qiter_t *t, void **el);
	errno_t (*el_free)(qiter_t *t, void *el);
	void (*free)(qiter_t *t);
};

struct qiter_s {
	void *p;
	const struct qiter_vtable_s *v;
};

static inline
errno_t qiter_copy(qiter_t *t, qiter_t *o) {
	assert(t);
	assert(t->v);
	assert(t->v->copy);
	assert(o);
	return t->v->copy(t, o);
}

static inline
errno_t qiter_seek(qiter_t *t, ptrdiff_t seek) {
	assert(t);
	assert(t->v);
	assert(t->v->seek);
	return t->v->seek(t, seek);
}

static inline
errno_t qiter_tell(qiter_t *t, size_t *pos) {
	assert(t);
	assert(t->v);
	assert(t->v->tell);
	return t->v->tell(t, pos);
}

static inline
errno_t qiter_size(qiter_t *t, size_t *size) {
	assert(t);
	assert(t->v);
	assert(t->v->size);
	return t->v->size(t, size);
}


_qiter_GENF(seek, ptrdiff_t seek)
	assert(o);
	return t->v->copy(t, seek);
}

_qiter_GENF(tell, qiter_t *o)
	assert(o);
	return t->v->copy(t, o);
}

_qiter_GENF(size, qiter_t *o)
	assert(o);
	return t->v->copy(t, o);
}

#endif /* SRC_QITER_QITER_H_ */
