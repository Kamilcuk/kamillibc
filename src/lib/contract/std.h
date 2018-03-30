/*
 * std.h
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#ifndef QQ_STD_H_
#define QQ_STD_H_

#include "require.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

typedef res_size_err_t result_size_t_errno_t_t;

void QQ_memset(void *dest, char c, size_t n)
	CONTRACT_require_nonnull(1)
	CONTRACT_require(n != 0);
void QQ_memcpy(void *dest, const void *src, size_t n)
	CONTRACT_require_nonnull(1,2)
	CONTRACT_require(n != 0);
void *QQ_malloc(size_t size)
	CONTRACT_require(size != 0);
void *QQ_calloc(size_t size)
	CONTRACT_require(size != 0);
void QQ_free(void **ptr)
	CONTRACT_require(ptr != 0);

result_size_t_errno_t_t QQ_vprintf(const char *format, va_list ap)
	CONTRACT_require_nonnull(1);
result_size_t_errno_t_t QQ_vfprintf(FILE *stream, const char *format, va_list ap)
	CONTRACT_require_nonnull(1,2);
result_size_t_errno_t_t QQ_vdprintf(int fd, const char *format, va_list ap)
	CONTRACT_require_nonnull(2);
result_size_t_errno_t_t QQ_vsprintf(char *str, const char *format, va_list ap)
	CONTRACT_require_nonnull(1,2);
result_size_t_errno_t_t QQ_vsnprintf(char *str, size_t size, const char *format, va_list ap);
result_size_t_errno_t_t QQ_printf(const char *format, ...);
result_size_t_errno_t_t QQ_fprintf(FILE *stream, const char *format, ...);
result_size_t_errno_t_t QQ_dprintf(int fd, const char *format, ...);
result_size_t_errno_t_t QQ_sprintf(char *str, const char *format, ...);
result_size_t_errno_t_t QQ_snprintf(char *str, size_t size, const char *format, ...);

res_int_err_t QQ_fcntl(int fildes, int cmd, ...);
result_size_t_errno_t_t QQ_pwrite(int fildes, const void *buf, size_t nbyte, off_t offset);
result_size_t_errno_t_t QQ_write(int fildes, const void *buf, size_t nbyte);
result_size_t_errno_t_t QQ_pread(int fildes, void *buf, size_t nbyte, off_t offset);
result_size_t_errno_t_t QQ_read(int fildes, void *buf, size_t nbyte);

/* and so on */

#endif /* QQ_STD_H_ */
