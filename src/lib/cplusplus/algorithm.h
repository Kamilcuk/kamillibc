/*
 * algorithm.h
 *
 *  Created on: 01.04.2018
 *      Author: michal
 */

#ifndef SRC_LIB_CPLUSPLUS_ALGORITHM_H_
#define SRC_LIB_CPLUSPLUS_ALGORITHM_H_

#include <stdbool.h>
#include <string.h>

#define ALGO_H_adjacent_find(N,ForwardIterator) \
		ForwardIterator * N##adjacent_find (ForwardIterator * first, ForwardIterator * last)
#define ALGO_C_adjacent_find(N,ForwardIterator) \
		ALGO_H_adjacent_find(N,ForwardIterator) \
{ \
  if (first != last) \
  { \
    ForwardIterator next=first; ++next; \
    while (next != last) { \
      if (*first == *next) \
        return first; \
      ++first; ++next; \
    } \
  } \
  return last; \
}

#define ALGO_H_find(N,T) \
	T N##find (T first, T const last, const T const val)
#define ALGO_C_find(N,T) \
	ALGO_H_find(N,T) \
{ \
  while (first!=last) { \
    if (!memcmp(first,val,sizeof(*first))) return first; \
    ++first; \
  } \
  return last; \
}



#define ALGO_H_for_each(N,InputIterator) \
	void N##for_each(InputIterator first, InputIterator const last, void (* const fn)(InputIterator el, void *arg), void *arg)
#define ALGO_C_for_each(N,InputIterator) \
	ALGO_H_for_each(N,InputIterator) \
{ \
  while (first!=last) { \
    fn (first, arg); \
    ++first; \
  } \
}



#define ALGO_H_search1(N,ForwardIterator1) \
	ForwardIterator1 N##search1(ForwardIterator1 first1, ForwardIterator1 const last1, \
                           ForwardIterator1 const first2, ForwardIterator1 const last2)
#define ALGO_C_search1(N,ForwardIterator1) \
	ALGO_H_search1(N,ForwardIterator1) \
{ \
  if (first2==last2) return first1; \
   \
  while (first1!=last1) \
  { \
    ForwardIterator1 it1 = first1; \
    ForwardIterator1 it2 = first2; \
    while (!memcmp(it1, it2, sizeof(*it1))) { \
        ++it1; ++it2; \
        if (it2==last2) return first1; \
        if (it1==last1) return last1; \
    } \
    ++first1; \
  } \
  return last1; \
}



#define ALGO_H_search2(N,ForwardIterator1,ForwardIterator2) \
	ForwardIterator1 * N##search2(ForwardIterator1 * first1, ForwardIterator1 * last1, \
                           ForwardIterator2 * first2, ForwardIterator2 * last2, \
						   bool (*pred)(ForwardIterator1 * el1, ForwardIterator2 * el2, void *arg), void *arg)
#define ALGO_C_search2(N,ForwardIterator1,ForwardIterator2) \
	ALGO_H_search2(N,ForwardIterator1,ForwardIterator2) \
{ \
  if (first2==last2) return first1; \
   \
  while (first1!=last1) \
  { \
    ForwardIterator1 * it1 = first1; \
    ForwardIterator2 * it2 = first2; \
    while (pred(it1, it2, arg)) { \
        ++it1; ++it2; \
        if (it2==last2) return first1; \
        if (it1==last1) return last1; \
    } \
    ++first1; \
  } \
  return last1; \
}



/*template <class InputIterator, class OutputIterator, class UnaryOperation>
  OutputIterator transform (InputIterator first1, InputIterator last1,
                            OutputIterator result, UnaryOperation op);*/
#define ALGO_H_transform1(N,InputIterator,OutputIterator) \
	OutputIterator * N##transform1(InputIterator * first1, InputIterator * const last1, \
                            OutputIterator * result, OutputIterator (* const op)(InputIterator * el, void *arg), void *arg)
#define ALGO_C_transform1(N,InputIterator,OutputIterator) \
	ALGO_H_transform1(N,InputIterator,OutputIterator) \
{ \
  while (first1 != last1) { \
	const OutputIterator tmp = op(first1, arg); \
    (void)memcpy(result, &tmp, sizeof(*result)); \
    ++result; ++first1; \
  } \
  return result; \
}


/*template <class InputIterator1, class InputIterator2,
          class OutputIterator, class BinaryOperation>
  OutputIterator transform (InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, OutputIterator result,
                            BinaryOperation binary_op);*/
#define ALGO_H_transform2(N,InputIterator1,InputIterator2,OutputIterator) \
	OutputIterator * N##transform2(InputIterator1 * first1, InputIterator1 * const last1, \
							InputIterator2 * first2, OutputIterator * result, \
							OutputIterator (* const op)(InputIterator1 * el1, InputIterator2 * el2, void *arg), void *arg)
#define ALGO_C_transform2(N,InputIterator1,InputIterator2,OutputIterator) \
	ALGO_H_transform2(N,InputIterator1,InputIterator2,OutputIterator) \
{ \
  while (first1 != last1) { \
	const OutputIterator tmp = op(first1, first2, arg); \
    (void)memcpy(result, &tmp, sizeof(*result)); \
    ++result; ++first1; ++first2; \
  } \
  return result; \
}



int algo_unittest();



#endif /* SRC_LIB_CPLUSPLUS_ALGORITHM_H_ */
