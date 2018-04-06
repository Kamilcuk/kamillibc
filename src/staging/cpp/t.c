/*
 * t.c
 *
 *  Created on: 30 mar 2018
 *      Author: kamil
 */
#include "t.h"

void T_DELETE(void *obj0)
{
	T_RTASSERT(obj0 != NULL);
	void ** const obj = obj0;
	void (** const destructor)(void *t0) = *obj;
	if (destructor != NULL) {
		(*destructor)(obj);
	}
	free(*obj);
	*obj = NULL;
}
