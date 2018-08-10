#ifndef SRC_BUILD_BUG_H_
#define SRC_BUILD_BUG_H_

#define BUILD_BUG_ON_NOT_POWER_OF_2(e)  \
	BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int __silence_pedantic; int:(-!!(e)); }) * 0)

#define BUILD_BUG_ON_INVALID(e) ((void)(sizeof((long)(e))))

#define BUILD_BUG_ON_MSG(condition, msg)  do { \
		if (condition) { \
			__attribute__((__error__(msg))) \
			extern void __build_bug_on_msg(void); \
			__build_bug_on_msg(); \
		} \
	} while(0)

#define BUILD_BUG_ON(condition) ((void)BUILD_BUG_ON_ZERO(condition))

#define BUILD_BUG()  BUILD_BUG_ON_MSG(1, "BUILD_BUG failed")

#endif	/* SRC_BUILD_BUG_H_ */
