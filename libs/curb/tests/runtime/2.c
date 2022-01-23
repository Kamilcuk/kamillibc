/*
 * main.c
 *
 *  Created on: 8 sie 2018
 *      Author: kamil
 */
#include <curb.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int func(int a) {
	curb(a > 0);
	return a;
}

#undef CURB_SEVERITY
#define CURB_SEVERITY warning

int main() {
	void *a = malloc(1);
	curb(a != NULL);
	free(a);

	curb(1);

	func(5);
	curbh(curbh_ignore, (printf("a"), 1));

	curb(printf("b") > 0);

	return 0;
}
