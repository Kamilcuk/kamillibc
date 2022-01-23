/*
 * findmsg_test.c
 *
 *  Created on: 20 mar 2018
 *      Author: kamil
 */
#include <findmsg/findmsg.h>
#include <findmsg/conf/newline.h>
#include <findmsg/conf/stub.h>
#include <assert.h>
#include <curb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEST(expr)  do{ \
	if (!(expr)) { \
		fprintf(stderr, "%s:%d: error: %s() expr %s failed\n", __FILE__, __LINE__, __func__, #expr); \
		return -1; \
	} \
}while(0)

static FILE **tempfiles = NULL;
static size_t tempfilescnt = 0;

static void tempfiles_cleanup(void) {
	for (size_t i = 0; i < tempfilescnt; ++i) {
		fclose(tempfiles[i]);
	}
	free(tempfiles);
	tempfiles = NULL;
	tempfilescnt = 0;
}

static int create_tmpfile(char content[], size_t size)
{
	FILE * const file = tmpfile();
	curb_exit(file != NULL);
	curb_exit(fwrite(content, 1, size, file) == size);
	curb_exit(fseek(file,  0, SEEK_SET) == 0);
	const int fd = fileno(file);
	curb_exit(fd > 0);

	if (tempfilescnt == 0) {
		atexit(tempfiles_cleanup);
	}
	tempfiles = realloc(tempfiles, sizeof(*tempfiles) * ++tempfilescnt);
	curb_exit(tempfiles != NULL);
	tempfiles[tempfilescnt - 1] = file;

	return fd;
}
#define CREATE_TMPFILE(str)  create_tmpfile(str, sizeof(str) - 1)

static inline int test_new_free_init(void)
{
	struct findmsg_s * f1 = findmsg_new(0, 16);
	findmsg_free(&f1);

	char buf[16];
	struct findmsg_s f2 = findmsg_INIT(0, buf, 16);
	(void)f2;

	struct findmsg_s f3 = findmsg_INIT_ON_STACK(0, 16);
	(void)f3;

	return 0;
}

static inline int test_recv_newline(void)
{
	char c[] = "aaa\nbbb\nccc\n";
	int fd = CREATE_TMPFILE(c);
	struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 16);

	struct timespec timeout = { 0, .tv_nsec = 30*1000*1000, };
	ssize_t linelen;
	char *in = strchr(c, '\n');
	for(int i = 0; (linelen = findmsg_findmsg(&f, &findmsg_conf_newline, NULL, &timeout)) > 0; ++i) {
		const char * const line = findmsg_msgpnt(&f);
		TEST(NULL != line);
		TEST(4 == linelen);
		TEST(in[0] == line[linelen-1]);
		TEST(in[-1] == line[linelen-2]);
		TEST(in[-2] == line[linelen-3]);
		TEST(in[-3] == line[linelen-4]);
		in = strchr(&in[1], '\n');
	}
	TEST(0 == linelen);
	return 0;
}

static inline int test_ending_badFd(void)
{
	struct findmsg_s f = findmsg_INIT_ON_STACK(-1, 16);
	ssize_t ret;
	ret = findmsg_beginning(&f, &findmsg_conf_stub, NULL, NULL);
	TEST(-1 == ret);
	ret = findmsg_ending(&f, &findmsg_conf_stub, NULL, 1, NULL);
	TEST(-1 == ret);
	ret = findmsg_findmsg(&f, &findmsg_conf_stub, NULL, NULL);
	TEST(-1 == ret);
	return 0;
}

struct test_conf_returingNegative_s {
	int ret;
};
static ssize_t test_conf_returingNegative_checkBeginning(const char buf[], size_t size, void *arg0)
{
	struct test_conf_returingNegative_s * arg = arg0;
	return arg->ret-- <= 0 ? -2000 : 0;
}
static int test_conf_returingNegative_checkEnding(const char buf[], size_t size, void *arg0)
{
	struct test_conf_returingNegative_s * arg = arg0;
	return arg->ret-- <= 0 ? -2000 : 0;
}
static inline int test_conf_returingNegative(void)
{
	struct findmsg_conf_s conf = {
			1, SIZE_MAX, test_conf_returingNegative_checkBeginning, test_conf_returingNegative_checkEnding
	};
	ssize_t ret;
	{
		struct test_conf_returingNegative_s val = {2};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_beginning(&f, &conf, &val, NULL);
		TEST(-2000 == ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_ending(&f, &conf, &val, 1, NULL);
		TEST(-2000 == ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_findmsg(&f, &conf, &val, NULL);
		TEST(-2000 == ret);
	}
	return 0;
}

int main()
{
	return test_new_free_init() ||
			test_recv_newline()  ||
			test_ending_badFd() ||
			test_conf_returingNegative();
}
