

/**
 * Works just like strtok, but does not modify input string
 * Instead it returns a pointer to the end of current string in curend.
 */
const char *const_strtok(const char *str, const char **strendptr, const char *delim, const char **saveptr);

int _const_strtok_test(void);

