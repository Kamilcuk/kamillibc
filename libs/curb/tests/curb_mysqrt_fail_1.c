#include <curb.h>
#include <stdio.h>
float mysqrt_in(float a) { return 0; }
__attribute__((__always_inline__))
static inline float mysqrt(float a) {
	curb(a >= 0);
	return mysqrt_in(a);
}
int main() {
	mysqrt(5);   // fine
	mysqrt(-5);  // compile time error
	float f;
	scanf("%f", &f);
	mysqrt(f);   // runtime error
}
