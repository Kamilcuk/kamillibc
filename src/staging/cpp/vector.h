/*
 * vector.h
 *
 *  Created on: 29 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CPP_VECTOR_H_
#define SRC_CPP_VECTOR_H_

#include "iterator.h"

#define _T_H_PREREQ_VECTOR_1(N,T)                                       _T_H_PREREQ_VECTOR_2(N,T,malloc)
#define  T_H_PREREQ_VECTOR(N,T,...)       BOOST_PP_OVERLOAD(_T_H_PREREQ_VECTOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_H_PREREQ_VECTOR_2(N,T,malloc) \
	T_H_FORWARDITERATOR(N,T)

#define _T_H_VECTOR_1(N,T)                                       _T_H_VECTOR_2(N,T,malloc)
#define  T_H_VECTOR(N,T,...)       BOOST_PP_OVERLOAD(_T_H_VECTOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_H_VECTOR_2(N,T,malloc) \
	\
	struct N##vector_s; \
	\
	typedef struct N##vector_vt_s { \
		_T_VF_MEMB(N,vector); \
		N##iterator (*begin)(struct N##vector_s *arg); \
		T * (*cbegin)(struct N##vector_s *arg); \
		N##iterator (*end)(struct N##vector_s *arg); \
		T * (*cend)(struct N##vector_s *arg); \
	} N##vector_vt; \
	\
	typedef struct N##vector_memb_s { \
		T * beg; \
		T * end; \
		T * alloc; \
	} N##vector_memb; \
	\
	typedef struct N##vector_s { \
		N##vector_vt vt; \
		N##vector_memb memb; \
	} N##vector; \
	\
	_T_VF_H(N,vector) \
	\
	void N##vector_constructor(struct N##vector_s *t); \
	void N##vector_constructor_2(struct N##vector_s *t, size_t count, T * beg); \
	void N##vector_constructor_4(struct N##vector_s *t, struct N##iterator_s *first, struct N##iterator_s *last); \
	\
	static inline N##iterator N##vector_begin(struct N##vector_s *t) { return t->vt.begin(t); } \
	static inline N##iterator N##vector_end(struct N##vector_s *t) { return t->vt.end(t); } \
	static inline T * N##vector_cbegin(struct N##vector_s *t) { return t->vt.cbegin(t); } \
	static inline T * N##vector_cend(struct N##vector_s *t) { return t->vt.cend(t); } \

#define _T_C_PREREQ_VECTOR_1(N,T)                                       _T_C_PREREQ_VECTOR_2(N,T,malloc)
#define  T_C_PREREQ_VECTOR(N,T,...)       BOOST_PP_OVERLOAD(_T_C_PREREQ_VECTOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_C_PREREQ_VECTOR_2(N,T,malloc) \
	T_C_FORWARDITERATOR(N,T)

#define _T_C_VECTOR_1(N,T)                                       _T_C_VECTOR_2(N,T,malloc)
#define  T_C_VECTOR(N,T,...)       BOOST_PP_OVERLOAD(_T_C_VECTOR_,__VA_ARGS__)(N,T,##__VA_ARGS__)
#define _T_C_VECTOR_2(N,T,malloc) \
	static N##iterator N##vector_begin_(struct N##vector_s *t) { \
		N##iterator ret = {0}; \
		N##ForwardIterator_constructor_pos(&ret, t->memb.beg); \
		return ret; \
	} \
	static N##iterator N##vector_end_(struct N##vector_s *t) { \
		N##iterator ret = {0}; \
		N##ForwardIterator_constructor_pos(&ret, t->memb.end); \
		return ret; \
	} \
	void N##vector_constructor(struct N##vector_s *t) { \
		*t = (struct N##vector_s){ \
			.vt.begin = &N##vector_begin_, \
			.vt.end = &N##vector_end_, \
		}; \
	} \
	void N##vector_constructor_2(struct N##vector_s *t, size_t count, T * beg) { \
		N##vector_constructor(t); \
		t->memb.beg = beg; \
		t->memb.end = &beg[count]; \
	} \
	void N##vector_constructor_4(struct N##vector_s *t, struct N##iterator_s *first, struct N##iterator_s *last) { \
		N##vector_constructor_2(t, N##iterator_distance(first, last), first->memb.pos); \
	}

#endif /* SRC_CPP_VECTOR_H_ */
