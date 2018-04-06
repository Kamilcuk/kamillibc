/*
 * t.h
 *
 *  Created on: 29 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CPP_T_H_
#define SRC_CPP_T_H_

#include <boost/preprocessor/facilities/overload.hpp>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define T_RTASSERT(expr)   assert( expr )
#define T_ASSERT_TYPESEQ(T1,T2)

#define T_NEW(T,constructor,...)  __extension__({ \
	T * _t = (T*)calloc(1, sizeof(T)); \
	(constructor)(_t, ##__VA_ARGS__); \
	_t; \
})

void T_DELETE(void *obj0);

static inline bool T_op_eq(void *t1, void *t2, size_t size) {
	return !memcmp(t1, t2, size);
}

static inline bool T_op_ne(void *t1, void *t2, size_t size) {
	return memcmp(t1, t2, size);
}

static inline void T_copy(void *t1, void *t2, size_t size) {
	(void)memcpy(t1, t2, size);
}


#define _T_VF_MEMB(N,class) \
	void (*destructor)(struct N##class##_s *t0)

#define _T_VF_H(N,class) \
	static inline bool N##class##_op_eq(struct N##class##_s *t, struct N##class##_s *t2) { \
		T_RTASSERT(t != NULL); T_RTASSERT(t2 != NULL); \
		return !T_op_eq(&t->memb, &t2->memb, sizeof(t->memb)); \
	} \
	static inline void N##class##_op_copy(struct N##class##_s *t, struct N##class##_s *t2) { \
		T_RTASSERT(t != NULL); T_RTASSERT(t2 != NULL); \
		T_copy(&t->memb, &t2->memb, sizeof(t->memb)); \
	}

#define _T_VF_C(N,class)

#ifndef _T_ESC
#undef _T_ESC
#define _T_ESC(...) __VA_ARGS__
#endif

#define _T_VF_INDIRECT_CALL_STATIC_INLINE_1(ret,N,obj,name) \
	static inline ret N##obj##_##name(struct N##obj##_s *t) { return t->vt.name(t); }
#define _T_VF_INDIRECT_CALL_STATIC_INLINE_2(ret,N,obj,name,types,args) \
	static inline ret N##obj##_##name(struct N##obj##_s *t, types) { return t->vt.name(t, args); }
#define _T_VF_INDIRECT_CALL_STATIC_INLINE(ret,N,obj,name,...) \
	BOOST_PP_OVERLOAD(_T_INDIRECT_CALL_STATIC_INLINE_,__VA_ARGS__)(ret,N,obj,name,##__VA_ARGS__)

#endif /* SRC_CPP_T_H_ */
