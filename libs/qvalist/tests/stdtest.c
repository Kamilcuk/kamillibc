#define QVA_EXT 1
#include <qvalist.h>
int main() {
	int arg3 = 5;

	int a1 = arg3 + 5;
	char a2 = 'c';
	float a3 = 1.1f;
	const char *a4 = "some string";
	qva_printf("%d %c %f %s\n", a1, a2, a3, a4);
}
//// C_STANDARD 99
//// C_STANDARD_REQUIRED YES
//// C_EXTENSIONS NO
//// PASS_REGULAR_EXPRESSION 10 c 1.100000 some string
