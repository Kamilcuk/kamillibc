#include <curb.h>
#include <stdio.h>
float mysqrt_in(float a) { return 0; }
#define mysqrt(a) \
		__extension__({ __typeof__(a) _a = a; curb(_a >= 0); mysqrt_in(_a); })
int main() {
	mysqrt(5);   // fine
	mysqrt(-5);  // compile time error
	float f;
	scanf("%f", &f);
	mysqrt(f);   // runtime error
}
