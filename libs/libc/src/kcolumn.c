#include "kcolumn.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

void kcolumn_print_row_cleanup_auto(size_t rownum, size_t cols, char *outstrs[cols],
				    const void *cookie) {
	for (size_t i = 0; i < cols; ++i) {
		free(outstrs[i]);
	}
}

int kcolumn_ext(const struct kcolumn_params *p) {
	int ret = -ENOMEM;
	struct table {
		char ***rows;
	};
	struct table table;
	table.rows = calloc(p->rowcnt, sizeof(*table.rows));
	if (!table.rows) {
		goto ERR_TABLE_ROWS;
	}
	size_t *collens = calloc(p->colcnt, sizeof(*collens));
	if (!collens) {
		goto ERR_COLLENS;
	}
	for (size_t i = 0; i < p->rowcnt; ++i) {
		table.rows[i] = calloc(p->colcnt, sizeof(char *));
		if (!table.rows[i]) {
			goto ERR_TABLE_ROWS_I;
		}
		if (p->print_row(i, p->colcnt, table.rows[i], p->cookie) != 0) {
			ret = -EFAULT;
			goto ERR_PRINT_ROW;
		}
		for (size_t j = 0; j < p->colcnt; ++j) {
			const size_t rowlen = strlen(table.rows[i][j]);
			if (collens[j] < rowlen) {
				collens[j] = rowlen;
			}
		}
	}
	const char *ORS = p->ORS ? p->ORS : "\n";
	const char *OFS = p->OFS ? p->OFS : " ";
	for (size_t i = 0; i < p->rowcnt; ++i) {
		for (size_t j = 0; j < p->colcnt; ++j) {
			const int r = fprintf(p->out, "%*s%s", (int)collens[j], table.rows[i][j],
					      j + 1 == p->colcnt ? ORS : OFS);
			if (r < 0) {
				ret = -EIO;
				goto ERR_FPRINTF;
			}
		}
	}
	ret = 0;
ERR_FPRINTF:
ERR_PRINT_ROW:
ERR_TABLE_ROWS_I:
	(void)0;
	void (*print_row_cleanup)(size_t rownum, size_t cols, char *outstrs[cols],
				  const void *cookie);
	print_row_cleanup =
	    p->print_row_cleanup ? p->print_row_cleanup : kcolumn_print_row_cleanup_auto;
	for (size_t i = 0; i < p->rowcnt; ++i) {
		if (table.rows[i]) {
			print_row_cleanup(i, p->colcnt, table.rows[i], p->cookie);
			free(table.rows[i]);
		}
	}
	free(collens);
ERR_COLLENS:
	free(table.rows);
ERR_TABLE_ROWS:
	return ret;
}

#if KAMILLIBC_TEST

#undef KAMILLIBC_TEST
#include "kasprintf.c"

typedef struct {
	int a;
	float b;
	const char *s;
} data_t;

int data_print_row(size_t rownum, size_t cols, char *outstrs[cols], const void *cookie) {
	assert(cols == 3);
	const data_t *data = cookie;
	if (kasprintf(&outstrs[0], "%d", data[rownum].a) < 0) {
		return -ENOMEM;
	}
	if (kasprintf(&outstrs[1], "%f", data[rownum].b) < 0) {
		free(outstrs[0]);
		return -ENOMEM;
	}
	if (kasprintf(&outstrs[2], "%s", data[rownum].s) < 0) {
		free(outstrs[0]);
		free(outstrs[1]);
		return -ENOMEM;
	}
	return 0;
}

int main() {
	// some data we are going to print
	static const data_t data[] = {
	    {10000, 1.0 / 3, "Hello"},
	    {1, 10000, "Och"},
	    {1, 2.0 / 3, "long string?"},
	};
	char *buf;
	size_t bufsize;
	FILE *out = open_memstream(&buf, &bufsize);
	if (!out)
		abort();
	int r = kcolumn(out, sizeof(data) / sizeof(*data), 3, data_print_row, data);
	if (r)
		return __LINE__;
	fclose(out);
	printf("%s", buf);
	free(buf);
}

#endif
