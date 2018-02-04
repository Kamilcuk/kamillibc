/*
 * tests.c
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <contract/contract.h>
#include <contract/result.h>

void test1() {
	struct a_s { int a; int b; };
	CONTRACT_Result_typedef(struct a_s,int) result_a_s_t;
	result_a_s_t result;
	result = CONTRACT_Result_ok(typeof(result), (struct a_s){1,2} );
	result = CONTRACT_Result_err(typeof(result), EINVAL );

	{
		struct a_s value = CONTRACT_Result_tryOk( result );
		(void)value;
	}
	{
		struct a_s value = CONTRACT_Result_tryOkElse( result, assert(0), (struct a_s){0,0} );
		(void)value;
	}
	{
		int value = CONTRACT_Result_tryErr( result );
		(void)value;
	}
	{
		int value = CONTRACT_Result_tryErrElse( result , 5 );
		(void)value;
	}
	{
		struct a_s value =
				CONTRACT_TRY result
				CONTRACT_TRYELSE assert(0); (struct a_s){0,0}; CONTRACT_TRYELSEEND;
		(void)value;
	}
}


void test2() {
	{
		result_size_t_errno_t_t res = QQ_write(STDOUT_FILENO, "aaaaa", 5);
		if (res.isErr) {
			result_size_t_errno_t_t res2 = QQ_fprintf(stderr, "write error %d\n", res.err);
			if (res2.isErr) {
				// aa fprintf failed
				assert(0);
			}
			assert(0);
		}
		printf("written %zu bytes\n", res.ok);
	}
	{
		errno = 0;
		size_t written = write(STDOUT_FILENO, "aaaaa", 5);
		if (errno) {
			// aaa write failed
			int errno2 = errno;
			errno = 0;
			fprintf(stderr, "write error %d\n", errno2);
			if (errno) {
				// aa fprintf failed
				assert(0);
			}
			assert(0);
		}
		printf("written %zu bytes\n", written);
	}

}
