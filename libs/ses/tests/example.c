/*
 * example.c
 *
 *  Created on: 31 sty 2020
 *      Author: kamil
 */
#define _POSIX_C_SOURCE 200809L
#include "../src/ses/ses.h"
#include <stdio.h>
#include <stdlib.h>

int print_hello(int argc, char **argv) {
	printf("Hello world!\n");
	return 0;
}

struct ses_cmds_s cmds[] = {
		{ .name = "print_hello", print_hello }
};

int main() {
	char *line = NULL;
	size_t size = 0;
	while (getline(&line, &size, stdin) > 0) {
		char *argv_space[20];
		ses_run_line(cmds, sizeof(cmds)/sizeof(*cmds),
				argv_space, sizeof(argv_space)/sizeof(*argv_space),
				line);
		return -1;
	}
	free(line);
}
