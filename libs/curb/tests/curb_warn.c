#include <curb.h>
#undef CURB_SEVERITY
#define CURB_SEVERITY warning
int main() {
	int a = 1;
	curb(a == 2);
	return 0;
}
