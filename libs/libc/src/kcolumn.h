
/*
 * @file
 * @author Kamil Cukrowski <kamil@leonidas>
 * @date 2021-07-12
 * SPDX-License-Identifier:
 */
#ifndef KCOLUMN_H_
#define KCOLUMN_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>

struct kcolumn_params {
	FILE *out;
	size_t rowcnt;
	size_t colcnt;
	int (*print_row)(size_t rownum, size_t cols, char *outstrs[cols], const void *cookie);
	const void *cookie;
	void (*print_row_cleanup)(size_t rownum, size_t cols, char *outstrs[cols], const void *cookie);
	const char *ORS;
	const char *OFS;
};

void kcolumn_print_row_cleanup_auto(size_t rownum, size_t cols, char *outstrs[cols], const void *cookie);

int kcolumn_ext(const struct kcolumn_params *p);

static inline
int kcolumn(FILE *out,
		size_t rowcnt, size_t colcnt,
		int (*print_row)(size_t rownum, size_t cols, char *outstrs[cols], const void *cookie),
		const void *cookie) {
	const struct kcolumn_params params = {
		.out = out,
		.rowcnt = rowcnt,
		.colcnt = colcnt,
		.print_row = print_row,
		.cookie = cookie,
	};
	return kcolumn_ext(&params);
}

#ifdef __cplusplus
}
#endif
#endif /* KCOLUMN_H_ */
