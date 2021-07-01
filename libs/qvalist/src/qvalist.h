/*
 * qvalist.h
 *
 *  Created on: 30.06.2021
 *      Author: Kamil Cukrowski
 */
#ifndef LIB_QVALIST_H_
#define LIB_QVALIST_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#if QVA_EXT
#include <stdio.h>
#endif

#if QVA_DEBUG
#include <stdio.h>
#define _qva_dbg(...)  fprintf(stderr, __VA_ARGS__)
#else
#define _qva_dbg(...)  do{}while(0)
#endif

// When QVA_CHECK_LEN is defined then:
// - qva_list::buf additionally stores a size_t with arguments size
// - the agrument size is checked in qva_arg and asserted on.
#if QVA_CHECK_LEN && !defined(NDEBUG)
#define _qva_ifcheck(...)  __VA_ARGS__
#else
// The macro is empty!
#define _qva_ifcheck(...)
#endif

// Assert on the expression and if on the expressin.
#define _qva_assertif(expr)  if (assert(!(expr)), expr)

typedef struct qva_list {
	// Stores memcpy of arguments.
	unsigned char *buf;
} qva_list;


// Copy from from to to size bytes just like memcpy
// and also increment the pointer pointed to by pntpnt by size.
static inline
void _qva_meminccpy(unsigned char **pntpnt, void *to, const void *from, size_t size) {
	memcpy(to, from, size);
	*pntpnt += size;
}
#define _qva_new_size_1( _1) \
		sizeof(_1)_qva_ifcheck(+sizeof(size_t))
#define _qva_new_size_2( _1,_2) \
		sizeof(_1)_qva_ifcheck(+sizeof(size_t))+sizeof(_2)_qva_ifcheck(+sizeof(size_t))
#define _qva_new_size_3( _1,_2,_3) \
		sizeof(_1)_qva_ifcheck(+sizeof(size_t))+sizeof(_2)_qva_ifcheck(+sizeof(size_t))+sizeof(_3)_qva_ifcheck(+sizeof(size_t))
#define _qva_new_size_4( _1,_2,_3,_4) \
		sizeof(_1)_qva_ifcheck(+sizeof(size_t))+sizeof(_2)_qva_ifcheck(+sizeof(size_t))+sizeof(_3)_qva_ifcheck(+sizeof(size_t))+sizeof(_4)_qva_ifcheck(+sizeof(size_t))
#define _qva_new_size_N(_1,_2,_3,_4, N, ...) \
		_qva_new_size_##N
#define _qva_new_size(...)  \
		_qva_new_size_N(__VA_ARGS__, 4,3,2,1)(__VA_ARGS__)

#define _qva_init_sizes_1( _1) \
		sizeof(_1)
#define _qva_init_sizes_2( _1,_2) \
		sizeof(_1),sizeof(_2)
#define _qva_init_sizes_3( _1,_2,_3) \
		sizeof(_1),sizeof(_2),sizeof(_3)
#define _qva_init_sizes_4( _1,_2,_3,_4) \
		sizeof(_1),sizeof(_2),sizeof(_3),sizeof(_4)
#define _qva_init_sizes_N(_1,_2,_3,_4, N, ...) \
		_qva_init_sizes_##N
#define _qva_init_sizes(...)  \
		_qva_init_sizes_N(__VA_ARGS__, 4,3,2,1)(__VA_ARGS__)

static inline
qva_list _qva_init_in(unsigned char *buf, size_t bufsize, const void *const refs[], const size_t sizes[]) {
	qva_list l = {buf};
	const void *const * iref = refs;
	const size_t *isize = sizes;
	unsigned char *pnt = l.buf;
	for (; *isize; iref++, isize++) {
		_qva_dbg("%s: put %d bytes to %p with bufsize=%d\n", __func__, (int)*isize, (void*)pnt, (int)bufsize);
		_qva_ifcheck(
				_qva_meminccpy(&pnt, pnt, isize, sizeof(*isize));
		)
		_qva_meminccpy(&pnt, pnt, *iref, *isize);
	}
	return l;
}

#ifndef __GNUC__

#define _qva_init_ref_1( _1) \
		&(_1)
#define _qva_init_ref_2( _1,_2) \
		&(_1),&(_2)
#define _qva_init_ref_3( _1,_2,_3) \
		&(_1),&(_2),&(_3)
#define _qva_init_ref_4( _1,_2,_3,_4) \
		&(_1),&(_2),&(_3),&(_4)
#define _qva_init_ref_N(_1,_2,_3,_4, N, ...) \
		_qva_init_ref_##N
#define _qva_init_ref(...)  \
		_qva_init_ref_N(__VA_ARGS__, 4,3,2,1)(__VA_ARGS__)
// Build arguments on stack.
#define _qva_init(buf, size, ...) \
		_qva_init_in(buf, size, (const void *const[]){_qva_init_ref(__VA_ARGS__)}, (const size_t[]){_qva_init_sizes(__VA_ARGS__), 0})
#else /* __GNUC__ */

#define _qva_init_def_ref_1( _1) \
		const __typeof__(_1) _tmp__1 = ( _1 );
#define _qva_init_def_ref_2( _1,_2) \
		const __typeof__(_1) _tmp__1 = ( _1 ); \
		const __typeof__(_2) _tmp__2 = ( _2 );
#define _qva_init_def_ref_3( _1,_2,_3) \
		const __typeof__(_1) _tmp__1 = ( _1 ); \
		const __typeof__(_2) _tmp__2 = ( _2 ); \
		const __typeof__(_3) _tmp__3 = ( _3 );
#define _qva_init_def_ref_4( _1,_2,_3,_4) \
		const __typeof__(_1) _tmp__1 = ( _1 ); \
		const __typeof__(_2) _tmp__2 = ( _2 ); \
		const __typeof__(_3) _tmp__3 = ( _3 ); \
		const __typeof__(_4) _tmp__4 = ( _4 );
#define _qva_init_def_ref_N(_1,_2,_3,_4, N, ...) \
		_qva_init_def_ref_##N
#define _qva_init_def_ref(...)  \
		_qva_init_def_ref_N(__VA_ARGS__, 4,3,2,1)(__VA_ARGS__)

#define _qva_init_ref_1( _1) \
		&(_tmp__1)
#define _qva_init_ref_2( _1,_2) \
		&(_tmp__1),&(_tmp__2)
#define _qva_init_ref_3( _1,_2,_3) \
		&(_tmp__1),&(_tmp__2),&(_tmp__3)
#define _qva_init_ref_4( _1,_2,_3,_4) \
		&(_tmp__1),&(_tmp__2),&(_tmp__3),&(_tmp__4)
#define _qva_init_ref_N(_1,_2,_3,_4, N, ...) \
		_qva_init_ref_##N
#define _qva_init_ref(...)  \
		_qva_init_ref_N(__VA_ARGS__, 4,3,2,1)(__VA_ARGS__)
// Create a function that uses __typeof__(arg) vat = arg and passes that to _qva_init_in.
// Why a function? Because the lifetime of compund literals is within ({ .. }) block, so 
// the compund literal has to be defined _outside_ of a block, while the rest has to be 
// inside.
#define _qva_init(_buf, size, ...) \
		({ \
			qva_list _qva_call(unsigned char *buf) { \
					_qva_init_def_ref(__VA_ARGS__); \
					return _qva_init_in(buf, size, (const void *const[]){_qva_init_ref(__VA_ARGS__)}, (const size_t[]){_qva_init_sizes(__VA_ARGS__), 0}); \
			} \
			_qva_call; \
		})(_buf)
#endif /* __GNUC__ */

/**
 * @define qva_new
 *
 * Constructs a qva_list variable using compund literal on stack.
 * If GNU C compiler extensions are available, then all arguments are interpreted as expressions
 * and assigned using `__typeof__(expr) var = (expr);` and then `&var` is taken and its value is
 * copied onto the buffer.
 * If GNU C compiler is not used, all arguments have to be variables, so that `&param` is possible.
 **/
#define qva_new(...) \
		_qva_init((unsigned char[_qva_new_size(__VA_ARGS__)]){0}, _qva_new_size(__VA_ARGS__), __VA_ARGS__)

/**
 * @define QVA_DEF_NEW
 * @brief Defines a temporary buffer and a qva_list variable with specified name.
 **/
#define QVA_DEF_NEW(variable, ...) \
		unsigned char _qva_##variable##_buffer[_qva_new_size(__VA_ARGS__)]; \
		qva_list variable = _qva_init(_qva_##variable##_buffer, sizeof(_qva_##variable##_buffer), __VA_ARGS__);

/* ------------------------------------------------------------------------- */

static inline
void _qva_copy(qva_list *dest, qva_list src) {
	*dest = src;
}

/**
 * @define qva_copy
 * @brief Copies from qva_list to another.
 **/
#define qva_copy(dest, src)  _qva_copy(&(dest), (src))

/**
 * Get's size bytes from the qva_list and places them in buf.
 * @returns buf
 **/
static inline
void *qva_argv(qva_list *l, void *buf, size_t size) {
	_qva_dbg("%s: getting %d bytes from %p\n", __func__, (int)size, (void*)l->buf);
	_qva_ifcheck(
			size_t s;
			_qva_meminccpy(&l->buf, &s, l->buf, sizeof(s));
			assert(s == size);
	)
	_qva_meminccpy(&l->buf, buf, l->buf, size);
	return buf;
}

/**
 * @define qva_arg
 * @brief Just like va_arg.
 **/
#define qva_arg(list, type) \
		*(type*)qva_argv(&list, (type[1]){0}, sizeof(type))

/**
 * @define qva_end
 * @brief To be compatible with va_end.
 **/
#define qva_end(list)   ((void)0)

/* ------------------------------------------------------------------------- */

#if QVA_EXT

static inline
int qva_qfprintf(FILE *out, const char *fmt, qva_list qva) {
	char c;
	int percent = 0;
	int sizeness = 0;
	int ret = -1;
	while ((c = *fmt++)) {
		if (percent) {
			switch(c) {
			case 'h':
				sizeness--;
				continue;
			case 'l':
				sizeness++;
				continue;
			case 'L':
				sizeness += 10;
				continue;
			case 'c':
				fprintf(out, "%c", qva_arg(qva, char));
				break;
			case 'd':
				fprintf(out, "%d", qva_arg(qva, int));
				break;
			case 'u':
				switch(sizeness) {
				case -2: fprintf(out, "%hhu", qva_arg(qva, unsigned char)); break;
				case -1: fprintf(out, "%hu", qva_arg(qva, unsigned short)); break;
				case 0: fprintf(out, "%u", qva_arg(qva, unsigned int)); break;
				case 1: fprintf(out, "%lu", qva_arg(qva, unsigned long)); break;
				case 2: fprintf(out, "%llu", qva_arg(qva, unsigned long long)); break;
				default: goto ERR;
				}
				break;
			case 'f':
				fprintf(out, "%f", qva_arg(qva, float));
				break;
			case 's':
				fprintf(out, "%s", qva_arg(qva, char *));
				break;
			case '%':
				fprintf(out, "%%");
				break;
			default:
				goto ERR;
			}
		}
		if (percent) {
			percent = 0;
			sizeness = 0;
			continue;
		}
		switch (c) {
		case '%':
			percent = 1;
			break;
		default:
			fprintf(out, "%c", c);
		}
	}
	ret = 0;
ERR:
	qva_end(qva);
	return ret;
}

#ifdef __GNUC__
// Get compiler to issue a warning if format string does not match parameters.
#define _qva_check_params(...)  ((void)(0?printf(__VA_ARGS__):0))
#else
#define _qva_check_params(...)  ((void)0)
#endif

#define qva_qprintf(fmt, qva_list)  qva_qfprintf(stdout, fmt, qva_list)
// Just like that - pass arguments to qva_new.
#define qva_fprintf(file, fmt, ...)  ( \
		_qva_check_params(fmt __VA_OPT__(,) __VA_ARGS__), \
		qva_qfprintf(file, fmt __VA_OPT__(,) qva_new(__VA_ARGS__)) \
		)
#define qva_printf(fmt, ...)  qva_fprintf(stdout, fmt __VA_OPT__(,) __VA_ARGS__)

#endif /* QVA_EXT */

#endif /* LIB_QVALIST_H_ */
// vim: ft=c

