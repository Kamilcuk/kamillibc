/*
 * newline.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#include "../conf/newline.h"

#include "../conf/stub.h"

#include <stdint.h>

const struct findmsg_conf_s findmsg_conf_newline = {
		.minlength = 1,
		.maxlength = SIZE_MAX,
		.checkBeginning = &findmsg_stub_checkBeginning,
		.checkEnding = &findmsg_conf_newline_checkEnding,
};

int findmsg_conf_newline_checkEnding(const char buf[], size_t size, void *arg)
{
	return buf[size-1] == '\n' ? findmsg_MSG_VALID : findmsg_MSG_TOO_SHORT;
}
