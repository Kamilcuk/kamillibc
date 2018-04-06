/*
 * cpp_test.c
 *
 *  Created on: 29 mar 2018
 *      Author: kamil
 */
#include "A_iterator.h"
#include "B_vector.h"
#include <unity/unity.h>

static void A_iterator_test() {
	struct bigs_s A[5] = {{1},{2},{3},{4},{5}};
	A_iterator * a = T_NEW(A_iterator, A_ForwardIterator_constructor_pos, &A[0]);
	T_DELETE(&a);
}

static void B_vector_test() {
	int A[5] = {1,2,3,4,5};
	B_vector * b;
	b = T_NEW(B_vector, B_vector_constructor_2, 5, A);
	for(B_iterator ite = B_vector_begin(b); !B_iterator_op_eq(&ite, (B_iterator[1]){B_vector_end(b)}); B_iterator_op_inc(&ite)) {
		printf("%p", B_iterator_deref(&ite));
	}
	T_DELETE(&b);
}

int cpp_test() {
	UnityBegin(__func__);
	RUN_TEST(A_iterator_test);
	RUN_TEST(B_vector_test);
	return UnityEnd();
}
