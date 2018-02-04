

#define TUPLE_T int, char, long double, char *, const char * const restrict
#define NAMESPACE(n) mytuple1_##n
#include "c_plus_plus/tuple.tc"

int a() {
	struct mytuple1_tuple_s a;
	a.elem_0 = 1;
	a.elem_1 = 'a';
	a.elem_2 = 0.4;
	a.elem_3 = "aaaaaaaaa";
	const char *str = mytuple1_get_3(&a);
	(void)str;
	return 0;
}

