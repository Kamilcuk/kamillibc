#include <extint.h>
#include <stdio.h>
#include <stdlib.h>
#define TEST(expr)  do{ if(!(expr)) exit(-1); }while(0)
int main() {
	extint_u5 a = extint_u5(1);
	extint_u5 b = extint_u5(2);

	extint_u5 c = extint_add_u5(a, b);
	TEST(c.v == 3);

	extint_u5 d = extint_add_u5(a, extint_u5(3));
	TEST(d.v == 4);

	extint_u5 f = extint_add(a, b);
	TEST(f.v == 3);

	extint_u3 e = extint_add_p(extint_u2(3), extint_u3(7));
	TEST(e.v == 2);
}
