#define QVA_EXT 1
#include <qvalist.h>
int main() {
	int arg3 = 5;
	qva_printf("%d %c %f %s\n",
				arg3 + 5,
				// Proper types have to be passed!
				(char)'c',
				1.1f,
				(char*)"some string"
	);
}
//// C_STANDARD 99
//// C_STANDARD_REQUIRED YES
//// C_EXTENSIONS YES
//// PASS_REGULAR_EXPRESSION 10 c 1.100000 some string
	
