/*
 * main.c
 *
 *  Created on: 8 sie 2018
 *      Author: kamil
 */
#include "../../../src/lib/curb.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int func(int a) {
	curb(a > 0);
	return a;
}

int main() {
	void *a = malloc(1);
	free(a);
	(void)curb(1);

	curb_handler_t curb_handler = curb_set_handler(curb_ignore_handler);
	func(-5);
	(void)curb((printf("a"), 0));
	curb_set_handler(curb_handler);

	curb(printf("b") > 0);

	return 0;
}
