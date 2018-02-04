# c-libs
C libraries written out of frustration

# Frustration

C is an amazing and simple language, fully powerfull and K&R did a great job.      
C commitee not always does a great job.    
What i think about C:  
- memcpy and memcmp should return void, not void *
- Most of the reasons why C sucks is because there are so many architectures to choose from.
 I think the newest C standards should choose which architectires to support, which will drop the count undefined behaviours. 
- error handing sucks. errno is a global variable. errno should be completly removed from existence. error value should be returned by return value by functions. POSIX does that implementing ssize_t and returning negative value for errors. However, functions return only -1 on error, and still set errno variable.
The best would be to look at the bests (Rust? C#?) implement something like Result<error_value_t,ok_value_t> and let C functions be implemented like Result<errno_t,size_t> write(int fd, const void *ptr, size_t s)
See src/contract for my best shot on how to implement it. 
- There should be an interface similar to C# Contracts, which will allow checking by the compiler
(if it's possible) the value of arguments passed to function. Like for example __attribute__((__nonnull__)) in gcc, but it should be able to check any constraint.
- multithreading sucks, because there is no good way of error handling.
- optimizing global variables like errno is very hard for the optimizer. Returing the value would optimize the code.
- All *_s() functions suck and shouldn't exists
- What to choose? strnlen or strnlen_s? I mean really, what to choose between size_t strnlen(const char *s, size_t maxlen); and size_t strnlen_s( const char *str, size_t strsz ); ?
- just returning the error value whould make all math functions simpler, so they wouldn't have to have exceptions
- We miss some kind of templates. However templates will be not possible in C. Sadly
- rsize_t is ok
- errno_t is ok