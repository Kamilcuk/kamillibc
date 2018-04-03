/*
 * stream.c
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */
#include <stream.h>

#include <errno.h>
#include <assert.h>

ssize_t _stream_ex(stream_const_func f, void *arg, void (*idle)(),
		const void *pnt, size_t chunksize, size_t nchunks, clock_t timeout)
{
	assert(pnt != NULL);
	assert(f != NULL);
	const clock_t start = clock();
	const size_t nchunkssav = nchunks;
	while(nchunks) {
		const ssize_t ret = f(arg, pnt, nchunks);
		if (ret < 0) {
			return ret;
		} else if (ret == 0) {
			if(idle) {
				idle();
			}
		} else {
			nchunks -= ret;
			pnt = (char*)pnt + (ret*chunksize);
			if (timeout != (clock_t)-1 && start+timeout > clock()) {
				return -ETIMEDOUT;
			}
		}
	}
	return nchunkssav - nchunks;
}

void stream_blocking_size_t_uint8_t(
		size_t (*process)(void *arg, uint8_t buf[], size_t nbyte), void *arg,
		void (*idle)(), uint8_t buf[], size_t nbyte)
{
	(void)_stream_ex((stream_const_func)process, arg, idle, buf, sizeof(buf[0]), nbyte, (clock_t)-1);
}

void stream_blocking_size_t_const_uint8_t(
		size_t (*process)(void *arg, const uint8_t buf[], size_t nbyte), void *arg,
		void (*idle)(), const uint8_t buf[], size_t nbyte)
{
	(void)_stream_ex((stream_const_func)process, arg, idle, buf, sizeof(buf[0]), nbyte, (clock_t)-1);
}
