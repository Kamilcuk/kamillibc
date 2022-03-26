
#define RI_NAME  mytype
#define RI_MAX 5
#define RI_MIN -2
#include <rangeint.h>

#define RI_NAME  type2
#define RI_MAX 10
#define RI_MIN -10
#include <rangeint.h>

#define RI_NAME  negtype
#define RI_MAX -10
#define RI_MIN -100
#include <rangeint.h>

#include <stdlib.h>
#include <stdio.h>

#define TEST(expr) do{ if(!(expr)) { fprintf(stderr, #expr " failed\n"); exit(-1); } }while(0)
int main() {
	mytype a = mytype_add(5, 1);
	TEST(a == -2);

	type2 b = type2_add(10, 1);
	TEST(b == -10);
	type2 c = type2_sub(-10, 10);
	TEST(c == 1);
	type2 d = type2_mul(10, 2);
	TEST(d == -1);

	{
		negtype a = negtype_from(-5);
		TEST(a == -96);
	}
}


