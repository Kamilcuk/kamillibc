#include <curb.h>
#include <math.h>

int mysqrt(int a) {
	curb_exit(a >= 0);
	return a / 2;
}

int main(int argc, char *argv[]) {
	mysqrt(argc - 2);
}
