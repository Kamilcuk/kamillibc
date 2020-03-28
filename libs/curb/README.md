# CURB

The C answer to constraint library. `curb(expr)` is used to check if expression is true.
For example:

```
   int a = 1;
   curb(a == 2); // compile time error
```

# Dependencies:

Needs a C99 compiler that supports GNU extensions:

- [statement expression](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)
- [nested functions](https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html#Nested-Functions)
- [`error` function attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes)

For the library to generate compile time errors optimization need to be enabled.
It is similar as for `_FORTIFY_SOURCE`.

# Usage:

For example can be used inside an `inline` function in current translation unit: 

```
#include <curb.h>
#include <stdio.h>

float mysqrt_in(float a);

static inline float mysqrt(float a) {
  curb(a >= 0);
  return mysqrt_in(a);
}

int main() {
   mysqrt(5);   // fine
   mysqrt(-5);  // compile time error
   float var;
   scanf("%f", &var);
   mysqrt(var);   // runtime error
}
```

It is nice to use a macro expansion, so that message is visible to the user in proper 
location. The library internally uses GCC extension statement expression anyway, so the best 
is to:

```
#include <curb.h>
float mysqrt_in(float a);
#define mysqrt(a) \
    ({ __typeof__(a) _a = a; curb(_a >= 0); mysqrt_in(a); })
```

The compile time error will generate:

```
test.c: In function 'main':
curb.h:54:3: error: call to '_curb_fail' declared with attribute error: 
test.c:4: error: curb("a >= 0") will fail
   54 |   _curb_fail(); \
      |   ^~~~~~~~~~~~
test.c:4:2: note: in expansion of macro 'curb'
    4 |  curb(a >= 0);
```

The runtime error message is meant to be similar to that of `assert` in glibc:

```
test.c:4: main: curb: constraint `a >= 2' failed
```

# curb_*

You can add a custom message and errno message:

```
int main() {
   int *a = malloc(sizeof(int));
   curb_msg(a != NULL, "allocating for a failed");
   curb_errno(a != NULL, ENOMEM);
   curb_msgerrno(a != NULL, "allocating for a failed", ENOMEM);
}
```

# Warning instead of a message

Because it's a macro, it's expanded on call side. 
You can change the macro `CURB_SEVERITY` in your code.

```
#undef CURB_SEVERITY
#define CURB_SEVERITY warning
int main() {
    int a = 1;
    curb(a == 2);  // will print compile time warning
    return 0;
}
```
 
# Author

Written by Kamil Cukrowski 2020
Licensed jointly under Beerware License and MIT License.