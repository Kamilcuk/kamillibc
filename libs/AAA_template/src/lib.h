/*
 * lib.h
 *
 *  Created on: 31 sty 2020
 *      Author: kamil
 */
#ifndef LIB_AAA_TEMPLATE_SRC_LIB_H_
#define LIB_AAA_TEMPLATE_SRC_LIB_H_

struct lib_s {
	int a;
};

static inline
int lib_return_me(struct lib_s *t, int a) {
	(void)t;
	return a;
}

#endif /* LIB_AAA_TEMPLATE_SRC_LIB_H_ */
