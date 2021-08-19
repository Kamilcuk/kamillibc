#define _GNU_SOURCE 1
#include "kasprintf.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int kasprintf(char **strp, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int r = kvasprintf(strp, fmt, va);
	va_end(va);
	return r;
}

int kvasprintf(char **strp, const char *fmt, va_list ap) {
#if HAVE_ASPRINTF
	return vasprintf(strp, fmt, ap);
#else
	*strp = NULL;
	return kvreasprintf(strp, fmt, ap);
#endif
}

int kreasprintf(char **strp, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int r = kvreasprintf(strp, fmt, va);
	va_end(va);
	return r;
}

int kvreasprintf(char **strp, const char *fmt, va_list ap) {
	assert(strp != NULL);
	const size_t curlen = *strp ? strlen(*strp) : 0;
	va_list ap2;
	va_copy(ap2, ap);
	const int len = vsnprintf(NULL, 0, fmt, ap2);
	va_end(ap2);
	assert(len >= 0);
	void *const pnt = realloc(*strp, curlen + len + 1);
	if (pnt == NULL) {
		return -1;
	}
	*strp = pnt;
	return vsprintf(*strp + curlen, fmt, ap);
}

#if KAMILLIBC_TEST

int main() {
	char *buf = NULL;
	kreasprintf(&buf, "Hello ");
	kreasprintf(&buf, "%s%c", "World", '!');
	int r = strcmp(buf, "Hello World!") == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
	printf(buf);
	free(buf);
	return r;
}

#endif
