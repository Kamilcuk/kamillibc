/*
 * container.h
 *
 *  Created on: 30 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CPP_TEST_CONTAINER_H_
#define SRC_CPP_TEST_CONTAINER_H_

#include "t.h"

#include "iterator.h"

#include <limits.h>
#include <stdint.h>

/*
class Base {
 public:
  virtual std::string Foo() = 0;
  virtual std::string Bar() = 0;
};

template <typename T>
class Derived : public Base {
 public:
  Derived(const T& data) : data_(data) { }

  virtual std::string Foo();
  virtual std::string Bar();

  T data() {
    return data_;
  }

 private:
  T data_;
};


;*/

#define T_H_std_string(N) \
	typedef char * N;

#define T_VT_base(N) \
	N##std_string (*N##foo)(); \
	N##std_string (*N##bar)();

#define T_MEMB_base(N)

#define T_H_base(N,NT) \
	\
	T_H_std_string(N##std_string) \
	\
	struct NT##_s; \
	\
	void N##constructor(struct NT##_s *t);

#define T_C_base(N,NT) \
	static void N##foo(struct NT##_s *t) { } \
	static void N##bar(struct NT##_s *t) { } \
	void N##constructor(struct NT##_s *t) { \
		t->vt.N##foo = N##foo; \
		t->vt.N##bar = N##bar; \
	}

#define T_VT_destructor(N) \
	void (*N##_destructor)(struct N##_s *t);

#define T_VT_derived(N,T) \
	T_VT_destructor(N) \
	T_VT_base(N##_base)

#define T_MEMB_derived(N,T) \
	T data_;

#define _T_H_DEC_CLASS(class,N,...) \
	\
	typedef struct N##_s N; \
	\
	struct N##_vt_s { \
		T_VT_##class(N,##__VA_ARGS__) \
	}; \
	\
	struct N##_memb_s { \
		T_MEMB_##class(N,##__VA_ARGS__) \
	}; \
	\
	struct N##_s { \
		struct N##_vt_s vt; \
		struct N##_memb_s memb; \
	}; \


#define T_H_derived(N,T) \
	\
	T_H_base(N##_base,N) \
	\
	_T_H_DEC_CLASS(derived,N,T) \
	\
	T N##constructor(struct N##_s *t); \
	T N##data(struct N##_s *t); \

#define T_C_derived(N,T) \
	\
	T_C_base(N##_base,N) \
	\
	T N##constructor(N *t) { \
		N##_base_constructor(t); \
	} \
	\
	T N##data(struct N##_s *t) { return t->memb.data_; }

#endif /* SRC_CPP_TEST_CONTAINER_H_ */
