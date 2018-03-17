/*
 * stub.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#include "stub.h"
#include <string.h>

const struct findmsg_conf_s FINDMSG_conf_stub = {
		.minlength = 0,
		.maxlength = SIZE_MAX,
		.checkBeginning = NULL,
		.checkEnding = NULL,
};

ssize_t findmsg_conf_stub_checkBeginning(const char buf[], size_t bufsize, void *arg)
{
	return findmsg_MSG_VALID;
}

int findmsg_conf_stub_checkEnding(const char buf[], size_t bufsize, void *arg)
{
	return findmsg_MSG_VALID;
}
