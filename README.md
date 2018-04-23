# kamillibs

C libraries written out of frustration  
Compile using CMake for Build/release/libkamil.so  

# Convention

- Naming:
     - Every structure declaration ends with '_s'
     - Every typedef declaration ends with '_t'
     - Every enum type ends with '_e'
     - Enum members, if enum type exists, are named like <enum_typename>_...
     - Every library has it's namespace, either lower or upper case
- Library namespace shouldn't change between lower and upper case
- Integer expression constants should be declared as enums
- Example library structure:<br>

```
enum lib_err_e {
	LIB_EOK = 0,
	LIB_ERROR,
	LIB_ENOBUF,
	...
};

struct lib_s {
	...
};

#define LIB_INIT(...) {...}

struct lib_s * lib_new(...);
void lib_free(const struct lib_s *t);

... lib_init(struct lib_s *t, ...);
void lib_deinit(struct lib_s *t);

int lib_test_unit();
int lib_test_integration(...);
int lib_test_assembly(...);
```

- The mean value of asserts per function should be 2
- Overloading macro on variable number of arguments should look like:

     _MACRO_1(arg1)              /* do smth with args */
     _MACRO_2(arg1, arg2)        /* do smth with args */
     _MACRO_3(arg1, arg2, arg3)  /* do smth with args */
     _MACRO_N(_1, _2, _3, N, ...)    _MACRO_##N
     MACRO(arg, ...)             _MACRO_N(arg, ##__VA_ARGS__, 3, 2, 1)(arg, ##__VA_ARGS__)
     
# Frustration

- error handing sucks. errno is a global variable. errno should be completly removed from existence. error value should be returned by return value by functions. POSIX does that implementing ssize_t and returning negative value for errors. However, functions return only -1 on error, and still set errno variable.
The best would be to look at the bests (Rust? C#?) implement something like Result<error_value_t,ok_value_t> and let C functions be implemented like Result<errno_t,size_t> write(int fd, const void *ptr, size_t s)
See result.h for my best shot on how to implement Result template and handle errors with assert 
- There should be an interface similar to C# Contracts, which will allow checking by the compiler
(if it's possible) the value of arguments passed to function. Like for example __attribute__((__nonnull__)) in gcc, but it should be able to check any constraint
See contract.h for my best shot of how to implement it.
- multithreading sucks, because there is no good way of error handling.
- optimizing global variables like errno is very hard for the optimizer. Returing the value would optimize the code.
- All *_s() functions suck and shouldn't exists. And they are not security enhancements of any kind.
- What to choose? strnlen or strnlen_s? I mean really, what to choose between size_t strnlen(const char *s, size_t maxlen); and size_t strnlen_s( const char *str, size_t strsz ); ?
- just returning the error value whould make all math functions simpler, so they wouldn't have to have exceptions
- rsize_t is ok
- errno_t is ok

# Author&License

Written by Kamil Cukrowski 2018   
Copyright jointly under MIT License and Beerware License  
See LICENSE file  