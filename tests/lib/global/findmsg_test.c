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

#define TEST_EQ(val, expr) do{ \
	if ((val) != (expr)) { \
		fprintf(stderr, "findmsg_unittest() error %s != %s\n", #val, #expr); \
		return -1; \
	} \
}while(0)

static int create_tmpfile(char content[], size_t size)
{
	FILE * const file = tmpfile();
	curb(file != NULL);
	curb(fwrite(content, 1, size, file) == size);
	curb(fseek(file,  0, SEEK_SET) == 0);
	const int fd = fileno(file);
	curb(fd > 0);
	return fd;
}
#define CREATE_TMPFILE(str)  create_tmpfile(str, sizeof(str))

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
		TEST_EQ(4, linelen);
		TEST_EQ(in[0], line[linelen-1]);
		TEST_EQ(in[-1], line[linelen-2]);
		TEST_EQ(in[-2], line[linelen-3]);
		TEST_EQ(in[-3], line[linelen-4]);
		in = strchr(&in[1], '\n');
	}
	TEST_EQ(0, linelen);
	return 0;
}

static inline int test_ending_badFd(void)
{
	struct findmsg_s f = findmsg_INIT_ON_STACK(-1, 16);
	ssize_t ret;
	ret = findmsg_beginning(&f, 1, findmsg_stub_checkBeginning, NULL, NULL);
	TEST_EQ(-1, ret);
	ret = findmsg_ending(&f, 1, 2, findmsg_stub_checkEnding, NULL, NULL);
	TEST_EQ(-1, ret);
	ret = findmsg_findmsg(&f, &findmsg_conf_stub, NULL, NULL);
	TEST_EQ(-1, ret);
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
		ret = findmsg_beginning(&f, 1, test_conf_returingNegative_checkBeginning, &val, NULL);
		TEST_EQ(-2000, ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_ending(&f, 1, 2, test_conf_returingNegative_checkEnding, &val, NULL);
		TEST_EQ(-2000, ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_findmsg(&f, &conf, &val, NULL);
		TEST_EQ(-2000, ret);
	}
	return 0;
}

int findmsg_unittest()
{
	return test_new_free_init() ||
			test_recv_newline()  ||
			test_ending_badFd() ||
			test_conf_returingNegative();
}
