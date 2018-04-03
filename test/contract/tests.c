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

	RESULT_typedef(struct a_s,int) result_a_s_t;

	result_a_s_t result;

	result = (result_a_s_t)RESULT_init_ok((struct a_s){1,2} );

	result = (result_a_s_t)RESULT_init_err(EINVAL);

	RESULT_set_ok(result, (struct a_s){1,2});

	RESULT_set_err(result, 2 );

	{
		struct a_s value = RESULT_tryOk( result );
		(void)value;
	}
	{
		struct a_s value = RESULT_tryOkElse(result, ({ assert(0), (struct a_s){0,0}; }) );
		(void)value;
	}
	{
		int value = RESULT_tryErr( result );
		(void)value;
	}
	{
		int value = RESULT_tryErrElse( result , 5 );
		value = RESULT_tryErrElse( result , ({ assert(0); 5; }) );
		(void)value;
	}
	{
		struct a_s value =
				RESULT_TRY result
				RESULT_TRYELSE assert(0); (struct a_s){0,0};
				RESULT_TRYELSEEND;
		(void)value;
	}
}

void test2() {
	{
		res_size_err_t res = QQ_write(STDOUT_FILENO, "aaaaa", 5);
		if (res.isErr) {
			res_size_err_t res2 = QQ_fprintf(stderr, "write error %d\n", res.err);
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

void test3() {
	{
		size_t res = RESULT_tryOk( QQ_write(STDOUT_FILENO, "aaaaa", 5) );
		(void)res;
	}
	{
		size_t res = RESULT_TRY
				QQ_write(STDOUT_FILENO, "aaaaa", 5)
		RESULT_TRYEND;
		(void)res;
	}
	{
		RESULT_typedef(char*, long double) res_charpnt_longdouble_t;
		struct a_s { int a, b; };
		RESULT_typedef(float*, struct a_s) res_floatpnt_astruct_t;
		RESULT_typedef(res_charpnt_longdouble_t, res_floatpnt_astruct_t) res_mix_t;
		res_mix_t mix1 = RESULT_init_ok( RESULT_init_ok(   "ABC" ) );
		res_mix_t mix2 = RESULT_init_ok( RESULT_init_err(  0.5 ) );
		res_mix_t mix3 = RESULT_init_err( RESULT_init_ok(  (float[]){0.1,0.2,0.4} ) );
		res_mix_t mix4 = RESULT_init_err( RESULT_init_err( (struct a_s){.a=1,.b=2} ) );
		(void)mix1; (void)mix2; (void)mix3; (void)mix4;
#if __GNUC__
		{
			char * res1 = RESULT_tryOk( RESULT_tryOk( mix1 ) );
			float * res2 = RESULT_tryOk( RESULT_tryErr( mix1 ) );
			long double res3 = RESULT_tryErr( RESULT_tryOk( mix1 ) );
			struct a_s res4 = RESULT_tryErr( RESULT_tryErr( mix1 ) );
			(void)res1; (void)res2; (void)res3; (void)res4;
		}
		{
			char * res1 = RESULT_tryOkElse(
					RESULT_tryOkElse( mix1,
							(res_charpnt_longdouble_t)RESULT_init_ok("ABC")
					), "ABC"
			);
			float * res2 = RESULT_tryOkElse(
					RESULT_tryErrElse( mix1,
							(res_floatpnt_astruct_t)RESULT_init_err((struct a_s){1,2})
					), (float[]){1.0,2.0}
			);
			long double res3 = RESULT_tryErrElse(
					RESULT_tryOkElse( mix1,
							(res_charpnt_longdouble_t)RESULT_init_err(0.4)
					), 0.4
			);
			struct a_s res4 = RESULT_tryErrElse(
					RESULT_tryErrElse( mix1,
							(res_floatpnt_astruct_t)RESULT_init_ok((float[]){0.0,0.1})
					), (struct a_s){1,2}
			);
			(void)res1; (void)res2; (void)res3; (void)res4;
		}
		{
			char * res1 =
					RESULT_TRY
						RESULT_TRY
							mix1
						RESULT_TRYEND
					RESULT_TRYEND;
			char * res2 =
					RESULT_TRY
						RESULT_TRY
							mix1
						RESULT_TRYELSE
							(res_charpnt_longdouble_t)RESULT_init_ok("ABC")
						RESULT_TRYELSEEND
					RESULT_TRYEND;
			char * res3 =
					RESULT_TRY
						RESULT_TRY
							mix1
						RESULT_TRYELSE
							(res_charpnt_longdouble_t)RESULT_init_err(0.1)
						RESULT_TRYELSEEND
					RESULT_TRYELSE
						"ABC"
					RESULT_TRYELSEEND;
			char * res4 =
					RESULT_TRY
						RESULT_TRY
							mix1
						RESULT_TRYEND
					RESULT_TRYELSE
						"ABC"
					RESULT_TRYELSEEND;
			(void)res1; (void)res2; (void)res3; (void)res4;
		}
#endif
	}
}
