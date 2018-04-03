/*
 * std.c
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#include "contract.h"
#include "std.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

void QQ_memset(void *dest, char c, size_t n)
{
	CONTRACT_assert(dest!=NULL);
	(void)memset(dest, c, n);
}

void QQ_memcpy(void *dest, const void *src, size_t n)
{
	CONTRACT_assert(dest!=NULL);
	CONTRACT_assert(src!=NULL);
	(void)memcpy(dest, src, n);
}

void * QQ_malloc(size_t size)
{
	CONTRACT_assert(size);
	void * ret = malloc(size);
	assert(ret != NULL);
	return ret;
}

void * QQ_calloc(size_t size)
{
	CONTRACT_assert(size);
	void * ret = malloc(size);
	assert(ret != NULL);
	QQ_memset(ret, 0, size);
	return ret;
}

void QQ_free(void **ptr)
{
	CONTRACT_assert(ptr!=NULL);
	if (*ptr) {
		free(*ptr);
	}
	*ptr = NULL;
}

result_size_t_errno_t_t QQ_vprintf(const char *format, va_list ap)
{
	CONTRACT_assert(format);
	errno = 0;
	const int ret = vprintf(format, ap);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

result_size_t_errno_t_t QQ_vfprintf(FILE *stream, const char *format, va_list ap)
{
	CONTRACT_assert(stream!=NULL);
	CONTRACT_assert(format!=NULL);
	errno = 0;
	const int ret = vfprintf(stream, format, ap);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

#if _POSIX_C_SOURCE >= 200809L
result_size_t_errno_t_t QQ_vdprintf(int fd, const char *format, va_list ap)
{
	CONTRACT_assert(format!=NULL);
	errno = 0;
	const int ret = vdprintf(fd, format, ap);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}
#endif

result_size_t_errno_t_t QQ_vsprintf(char *str, const char *format, va_list ap)
{
	CONTRACT_assert(str!=NULL);
	CONTRACT_assert(format!=NULL);
	errno = 0;
	const int ret = vsprintf(str, format, ap);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

result_size_t_errno_t_t QQ_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	CONTRACT_assert(str!=NULL);
	CONTRACT_assert(size);
	CONTRACT_assert(format!=NULL);
	errno = 0;
	const int ret = vsnprintf(str, size, format, ap);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

result_size_t_errno_t_t QQ_printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const result_size_t_errno_t_t ret = QQ_vprintf(format, ap);
	va_end(ap);
	return ret;
}

result_size_t_errno_t_t QQ_fprintf(FILE *stream, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const result_size_t_errno_t_t ret = QQ_fprintf(stream, format, ap);
	va_end(ap);
	return ret;
}

result_size_t_errno_t_t QQ_dprintf(int fd, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const result_size_t_errno_t_t ret = QQ_dprintf(fd, format, ap);
	va_end(ap);
	return ret;
}

result_size_t_errno_t_t QQ_sprintf(char *str, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const result_size_t_errno_t_t ret = QQ_sprintf(str, format, ap);
	va_end(ap);
	return ret;
}

result_size_t_errno_t_t QQ_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const result_size_t_errno_t_t ret = QQ_snprintf(str, size, format, ap);
	va_end(ap);
	return ret;
}

#if _XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200809L
result_size_t_errno_t_t QQ_pwrite(int fildes, const void *buf, size_t nbyte, off_t offset)
{
	CONTRACT_assert_nonnull(buf);
	errno = 0;
	ssize_t ret = pwrite(fildes, buf, nbyte, offset);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}
#endif

result_size_t_errno_t_t QQ_write(int fildes, const void *buf, size_t nbyte)
{
	CONTRACT_assert_nonnull(buf);
	errno = 0;
	ssize_t ret = write(fildes, buf, nbyte);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

#if _XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200809L
result_size_t_errno_t_t QQ_pread(int fildes, void *buf, size_t nbyte, off_t offset)
{
	CONTRACT_assert_nonnull(buf);
	errno = 0;
	ssize_t ret = pread(fildes, buf, nbyte, offset);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}
#endif

result_size_t_errno_t_t QQ_read(int fildes, void *buf, size_t nbyte)
{
	CONTRACT_assert_nonnull(buf);
	errno = 0;
	ssize_t ret = read(fildes, buf, nbyte);
	return errno ?
			(result_size_t_errno_t_t)RESULT_init_err(errno) :
			(result_size_t_errno_t_t)RESULT_init_ok(ret);
}

