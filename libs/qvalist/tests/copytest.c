#include <qvalist.h>
#include <stdio.h>
int main() {
	int a1 = 10;
	char a2 = 'c';
	float a3 = 1.1f;
	const char *a4 = "some string";
	qva_list qva = qva_new(a1, a2, a3, a4);
	printf("%d ", qva_arg(qva, int));
	printf("%c ", qva_arg(qva, char));
	qva_list qva2;
	qva_copy(qva2, qva);
	printf("%f ", qva_arg(qva, float));
	(void)qva_arg(qva2, float);
	(void)qva_arg(qva, char*);
	printf("%s\n", qva_arg(qva2, char*));
	qva_end(qva);
	qva_end(qva2);
}
//// C_STANDARD 99
//// C_STANDARD_REQUIRED YES
//// C_EXTENSIONS NO
//// PASS_REGULAR_EXPRESSION 10 c 1.100000 some string
