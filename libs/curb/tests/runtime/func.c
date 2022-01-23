#include <curb.h>
static inline void func(int a) {
	curb(a == 42);
}
int main() {
	func(42);
}
