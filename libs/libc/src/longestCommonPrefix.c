#include "longestCommonPrefix.h"
#include <assert.h>
#include <string.h>

void longestCommonPrefix(size_t strscnt, const char *const strs[strscnt], const char **ret, size_t *retlen) {
	assert(strscnt);
	assert(strs);
	assert(ret);
	assert(retlen);
	
	const char *cur = strs[1];
	size_t curlen = strlen(cur);
	for (const char *const *i = strs, *const *const max = i + strscnt; i < max; ++i) {
		const char *istr = *i;
		const size_t ilen = strlen(istr);
		if (ilen < curlen) {
			curlen = ilen;
		}
		while (curlen) {
			if (memcmp(cur, istr, curlen) == 0) {
				break;
			}
			curlen--;
			if (!curlen) {
				cur = NULL;
				goto EXIT;
			}
		}
	}
EXIT:
	*ret = cur;
	*retlen = curlen;
}

#if KAMILLIBC_TEST
#include <stdio.h>
#define TEST(x)  do { if(!(x)) return __LINE__; } while(0)
#define TESTF(res, ...)  do { \
	const char *const strs[] = { __VA_ARGS__ }; \
	const char *pref; \
	size_t preflen; \
	longestCommonPrefix(sizeof(strs)/sizeof(*strs), strs, &pref, &preflen); \
	if (pref) printf("%zu %.*s\n", preflen, (int)preflen, pref); \
	TEST(preflen == strlen(res)); \
	if (pref) TEST(memcmp(pref, res, preflen) == 0); \
} while(0)
int main() {
	TESTF("fl", "flower", "flow", "flight");
	TESTF("f", "fower", "flow", "flight");
	TESTF("", "aflower", "flow", "flight");
	TESTF("", "dog","racecar","car");
	TESTF("dog", "dog","dog","dog");
}
#endif

