/*
 * memcmp.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FINDMSG_MEMCMP_H_
#define SRC_FINDMSG_MEMCMP_H_

#include <findmsg/types.h>
#include <findmsg/findmsg.h>

#include <assert.h>

ssize_t findmsg_conf_memcmp_checkBeginning(const char buf[], size_t bufsize, void *arg);
int findmsg_conf_memcmp_checkEnding(const char buf[], size_t bufsize, void *arg);

static inline struct findmsg_conf_s findmsg_conf_memcmp(size_t memsize)
{
	struct findmsg_conf_s ret = {
			.minlength = 1,
			.maxlength = memsize,
			.checkBeginning = &findmsg_conf_memcmp_checkBeginning,
			.checkEnding = &findmsg_conf_memcmp_checkEnding
	};
	return ret;
}

static inline ssize_t findmsg_memcmp(struct findmsg_s *t, const char mem[], size_t memsize, clock_t *timeout)
{
	assert(memsize);
	const struct findmsg_conf_s conf = findmsg_conf_memcmp(memsize);
	return findmsg_findmsg(t,
			&conf, (void*)mem,
			timeout);
}

#endif /* SRC_FINDMSG_MEMCMP_H_ */
