/*
 * builtin.c
 *
 *  Created on: 17 kwi 2018
 *      Author: kamil
 */
#include <stdio.h>
#include <stdlib.h>

int ses_builtin_echo(int argc, char *argv[])
{
	if (--argc) {
		printf("%s", (++argv)[0]);
		while(--argc) {
			printf(" ");
			printf("%s", (++argv)[0]);
		}
	}
	printf("\n");
	return 0;
}

int ses_builtin_abort(int argc, char *argv[])
{
	abort();
	return 0;
}

int ses_builtin_exit(int argc, char *argv[])
{
	if (argc > 2) {
		exit(atoi(argv[1]));
	} else {
		exit(0);
	}
	return 0;
}

static int ses_builtin_testargs(int argc, char *argv[]) {
	for(int i = 0; i < argc; ++i) {
		printf("argv[%d] = '%s'\n", i, argv[i]);
	}
	return argc;
}
