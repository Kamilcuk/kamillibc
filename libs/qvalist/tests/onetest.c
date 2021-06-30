#define QVA_EXT 1
#include <qvalist.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>


#define TEST(expr)  assert(expr)
int main() {
	int arg1 = 10;
	float arg2 = 20.2;
	qva_list qva = qva_new(arg1, arg2);
	TEST(qva_arg(qva, int) == arg1);
	TEST(qva_arg(qva, float) == arg2);
	qva_end(qva);

	int arg3 = 5;
	qva_printf("%d %c %f %s\n",
				arg3 + 5,
				// Proper types have to be passed!
				(char)'c',
				1.1f,
				(char*)"some string"
	);
}
	
