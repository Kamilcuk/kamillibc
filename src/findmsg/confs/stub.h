/*
 * stub.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FINDMSG_STUB_H_
#define SRC_FINDMSG_STUB_H_

#include <findmsg/findmsg.h>

extern const struct findmsg_conf_s FINDMSG_conf_stub;

ssize_t findmsg_conf_stub_checkBeginning(const char buf[], size_t bufsize, void *arg);
int findmsg_conf_stub_checkEnding(const char buf[], size_t bufsize, void *arg);

int findmsg_conf_memcmp_checkBeginning(const char buf[], size_t bufsize, void *arg);
int findmsg_conf_memcmp_checkEnding(const char buf[], size_t bufsize, void *arg);

#endif /* SRC_FINDMSG_STUB_H_ */
