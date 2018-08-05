/*
 * posix.c
 *
 *  Created on: 5 sie 2018
 *      Author: kamil
 */
#include <poll.h>
#include <errno.h>
#include <time.h>

int poll (struct pollfd *__fds, nfds_t __nfds, int __timeout)
{
	return -ENOSYS;
}
