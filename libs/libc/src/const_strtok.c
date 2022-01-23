#include <string.h>
#include <stdlib.h>

/* Reentrant string tokenizer.  Generic version.
   Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
// modified from https://github.com/lattera/glibc/blob/master/string/strtok_r.c

const char *const_strtok(const char *restrict str, const char **restrict strendptr, const char *restrict delim, const char **restrict saveptr)
{
  const char *end;

  if (str == NULL)
    str = *saveptr;

  if (*str == '\0')
    {
      *saveptr = str;
      return NULL;
    }

  /* Scan leading delimiters.  */
  str += strspn (str, delim);
  if (*str == '\0')
    {
      *saveptr = str;
      return NULL;
    }

  /* Find the end of the token.  */
  end = str + strcspn (str, delim);
  if (*end == '\0')
    {
      *strendptr = *saveptr = end;
      return str;
    }

  /* Terminate the token and make *SAVE_PTR point past it.  */
  *strendptr = end;
  *saveptr = end + 1;
  return str;
}

int _const_strtok_test(void) {
	const char str[] = "  str1      str2   \t\nstr3";
	const char delim[] = " \t\n";
	const char *token, *end = NULL, *save;
	token = const_strtok(str, &end, delim, &save);
	if (memcmp(token, "str1", end - token)) return __LINE__;
	token = const_strtok(NULL, &end, delim, &save);
	// printf("'%.*s`\n", (int)(end - token), token);
	if (memcmp(token, "str2", end - token)) return __LINE__;
	token = const_strtok(NULL, &end, delim, &save);
	if (memcmp(token, "str3", end - token)) return __LINE__;
	token = const_strtok(NULL, &end, delim, &save);
	if (token) return __LINE__;
	return 0;
}

