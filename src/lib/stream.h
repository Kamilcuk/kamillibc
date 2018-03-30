/*
 * stream.h
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CLIBS_STREAM_H_
#define SRC_CLIBS_STREAM_H_

#include <unistd.h>
#include <stdint.h>
#include <time.h>

typedef ssize_t (*stream_const_func)(void *arg, const void *pnt, size_t nchunks);
typedef ssize_t (*stream_func)(void *arg, void *pnt, size_t nchunks);

struct stream_s {
	stream_const_func write;
	stream_func read;
};

#define stream_write(stream, memb, pnt, nchunks)  _stream_write_call(&(stream)->(memb), (stream), (pnt), (nchunks))

static inline ssize_t _stream_write_call(struct stream_s *t, void *arg, const void *pnt, size_t nchunks)
{
	return t->write(arg, pnt, nchunks);
}

#define stream_read(stream, memb, pnt, nchunks)   _stream_read_call(&(stream)->(memb), (stream), (pnt), (nchunks))

static inline ssize_t _stream_read_call(struct stream_s *t, void *arg, void *pnt, size_t nchunks)
{
	return t->read(arg, pnt, nchunks);
}

ssize_t _stream_ex(stream_const_func f, void *arg, void (*idle)(),
		const void *pnt, size_t chunksize, size_t nchunks, clock_t timeout);

void stream_blocking_size_t_uint8_t(
		size_t (*process)(void *arg, uint8_t buf[], size_t nbyte), void *arg,
		void (*idle)(), uint8_t buf[], size_t nbyte);
void stream_blocking_size_t_const_uint8_t(
		size_t (*process)(void *arg, const uint8_t buf[], size_t nbyte), void *arg,
		void (*idle)(), const uint8_t buf[], size_t nbyte);

#endif /* SRC_CLIBS_STREAM_H_ */
