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
#include <unity/unity.h>
#include <try.h>
#include <unistd.h>
#include <string.h>

static int create_tmpfile(char content[], size_t size)
{
	FILE * const file = tmpfile();
	assert(file);
	try( fwrite(content, 1, size, file) == size );
	try( fseek(file,  0, SEEK_SET) == 0 );
	const int fd = fileno(file);
	assert(fd > 0);
	return fd;
}
#define CREATE_TMPFILE(str)  create_tmpfile(str, sizeof(str))

static void test_new_free_init()
{
	struct findmsg_s * f1 = findmsg_new(0, 16);
	findmsg_free(&f1);

	char buf[16];
	struct findmsg_s f2 = findmsg_INIT(0, buf, 16);
	(void)f2;

	struct findmsg_s f3 = findmsg_INIT_ON_STACK(0, 16);
	(void)f3;
}

static void test_recv_newline()
{
	char c[] = "aaa\nbbb\nccc\n";
	int fd = CREATE_TMPFILE(c);
	struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 16);

	char *line;
	clock_t timeout = 30;
	ssize_t linelen;
	char *in = strchr(c, '\n');
	for(int i = 0; (linelen = findmsg_findmsg(&f, &findmsg_conf_newline, NULL, &timeout, &line)) > 0; ++i) {
		TEST_ASSERT_EQUAL(4, linelen);
		TEST_ASSERT_EQUAL(in[0], line[linelen-1]);
		TEST_ASSERT_EQUAL(in[-1], line[linelen-2]);
		TEST_ASSERT_EQUAL(in[-2], line[linelen-3]);
		TEST_ASSERT_EQUAL(in[-3], line[linelen-4]);
		in = strchr(&in[1], '\n');
	}
	TEST_ASSERT_EQUAL(0, linelen);
}

static void test_ending_badFd()
{
	struct findmsg_s f = findmsg_INIT_ON_STACK(-1, 16);
	ssize_t ret;
	ret = findmsg_beginning(&f, 1, findmsg_stub_checkBeginning, NULL, NULL);
	TEST_ASSERT_EQUAL(-1, ret);
	ret = findmsg_ending(&f, 1, 2, findmsg_stub_checkEnding, NULL, NULL);
	TEST_ASSERT_EQUAL(-1, ret);
	ret = findmsg_findmsg(&f, &findmsg_conf_stub, NULL, NULL, NULL);
	TEST_ASSERT_EQUAL(-1, ret);
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
static void test_conf_returingNegative()
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
		TEST_ASSERT_EQUAL(-2000, ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_ending(&f, 1, 2, test_conf_returingNegative_checkEnding, &val, NULL);
		TEST_ASSERT_EQUAL(-2000, ret);
	}
	{
		struct test_conf_returingNegative_s val = {0};
		char c[] = "aaa\nbbb\nccc\n";
		int fd = CREATE_TMPFILE(c);
		struct findmsg_s f = findmsg_INIT_ON_STACK(fd, 32);
		ret = findmsg_findmsg(&f, &conf, &val, NULL, NULL);
		TEST_ASSERT_EQUAL(-2000, ret);
	}
}

int findmsg_unittest()
{
	UnityBegin(__func__);
	RUN_TEST(test_new_free_init);
	RUN_TEST(test_recv_newline);
	RUN_TEST(test_ending_badFd);
	RUN_TEST(test_conf_returingNegative);
	return UnityEnd();
}
