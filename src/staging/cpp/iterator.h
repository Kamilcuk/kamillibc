/*
 * iterator.h
 *
 *  Created on: 29 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CPP_ITERATOR_C_
#define SRC_CPP_ITERATOR_C_

#include "t.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define T_H_DISTANCE(N) \
	size_t N##iterator_distance(struct N##iterator_s *first, struct N##iterator_s *last);

#define T_C_DISTANCE(N) \
	size_t N##iterator_distance(struct N##iterator_s *first, struct N##iterator_s *last) { \
		return last->memb.pos - first->memb.pos; \
	}

#define _T_H_ITERATOR_1(N,Category,T)                                  _T_H_ITERATOR_2(N,Category,T,ptrdiff_t)
#define _T_H_ITERATOR_2(N,Category,T,Distance)                         _T_H_ITERATOR_3(N,Category,T,Distance,T *)
#define  T_H_ITERATOR(N,Category,T,...)  BOOST_PP_OVERLOAD(_T_H_ITERATOR_,__VA_ARGS__)(N,Category,T,##__VA_ARGS__)
#define _T_H_ITERATOR_3(N,Category,T,Distance,Pointer) \
	\
	enum N##iterator_categories_e { \
		N##iterator_input_iterator_tag, \
		N##iterator_output_iterator_tag, \
		N##iterator_forward_iterator_tag, \
		N##iterator_bidirectional_iterator_tag, \
		N##iterator_random_access_iterator_tag, \
	}; \
	\
	struct N##iterator_s; \
	typedef struct N##iterator_vt_s { \
		_T_VF_MEMB(N,iterator); \
		void (*advance)(struct N##iterator_s *t, size_t n); \
	} N##iterator_vt; \
	\
	typedef struct N##iterator_memb_s { \
		Pointer pos; \
	} N##iterator_memb; \
	\
	typedef struct N##iterator_s { \
		N##iterator_vt vt; \
		N##iterator_memb memb; \
	} N##iterator; \
	\
	T_H_DISTANCE(N) \
	\
	_T_VF_H(N,iterator) \
	\
	static inline void N##iterator_advance(struct N##iterator_s *t, size_t n) { return t->vt.advance(t, n); } \
	static inline void N##iterator_op_inc(struct N##iterator_s *t) { N##iterator_advance(t, +1); } \
	static inline void N##iterator_op_dec(struct N##iterator_s *t) { N##iterator_advance(t, -1); } \
	static inline enum N##iterator_categories_e N##iterator_get_tag(struct N##iterator_s *t) { return N##iterator_##Category; } \
	static inline T * N##iterator_deref(struct N##iterator_s *t) { return t->memb.pos; }

#define _T_C_ITERATOR_1(N,Category,T)                                  _T_C_ITERATOR_2(N,Category,T,ptrdiff_t)
#define _T_C_ITERATOR_2(N,Category,T,Distance)                         _T_C_ITERATOR_3(N,Category,T,Distance,T *)
#define  T_C_ITERATOR(N,Category,T,...)  BOOST_PP_OVERLOAD(_T_C_ITERATOR_,__VA_ARGS__)(N,Category,T,##__VA_ARGS__)
#define _T_C_ITERATOR_3(N,Category,T,Distance,Pointer) \
	T_C_DISTANCE(N)

#define _T_H_FORWARDITERATOR_1(N,T)                                  _T_H_FORWARDITERATOR_2(N,T,ptrdiff_t)
#define _T_H_FORWARDITERATOR_2(N,T,Distance)                         _T_H_FORWARDITERATOR_3(N,T,Distance,T *)
#define  T_H_FORWARDITERATOR(N,T,...)  BOOST_PP_OVERLOAD(_T_H_FORWARDITERATOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_H_FORWARDITERATOR_3(N,T,Distance,Pointer) \
	\
	T_H_ITERATOR(N,forward_iterator_tag,T) \
	\
	typedef N##iterator N##ForwardIterator; \
	\
	void N##ForwardIterator_constructor_pos(struct N##iterator_s *t, T * pos);

#define _T_C_FORWARDITERATOR_1(N,T)                                  _T_C_FORWARDITERATOR_2(N,T,ptrdiff_t)
#define _T_C_FORWARDITERATOR_2(N,T,Distance)                         _T_C_FORWARDITERATOR_3(N,T,Distance,T *)
#define  T_C_FORWARDITERATOR(N,T,...)  BOOST_PP_OVERLOAD(_T_C_FORWARDITERATOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_C_FORWARDITERATOR_3(N,T,Distance,Pointer) \
	\
	T_C_ITERATOR(N,forward_iterator_tag,T) \
	\
	static void N##ForwardIterator_advance(struct N##iterator_s *t, size_t n) { \
		t->memb.pos += n; \
	} \
	\
	void N##ForwardIterator_constructor_pos(struct N##iterator_s *t, T * pos) { \
		t->memb.pos = pos; \
		t->vt.advance = &N##ForwardIterator_advance; \
	}

#endif /* SRC_CPP_ITERATOR_C_ */
