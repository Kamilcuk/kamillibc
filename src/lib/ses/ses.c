/*
 * ses.c
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */
#define _POSIX_C_SOURCE 200809L

#include <ses/ses.h>

#if SES_USE_ao_string_tokenize
#include "tokenize/ao_string_tokenize.h"
#endif

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <time.h>

static inline int ses_tokenize_ao_string_tokenize(char *argv[restrict], size_t argvsize, char arg[restrict])
{
#if !SES_USE_ao_string_tokenize
	typedef struct {
		int tkn_ct;
		char **tkn_list;
	} token_list_t;
	extern token_list_t * ao_string_tokenize(char *);
#endif
	int argc;
	const token_list_t * const p = ao_string_tokenize(arg);
	if (p == NULL) {
		fprintf(stderr, "Parsing string ``%s'' failed:\n\terrno %d\n", arg, errno);
		argc = SES_ENOMEM;
	} else {
		//printf("Parsed string ``%s''\ninto %d tokens:\n", arg, p->tkn_ct);
		argc = p->tkn_ct;
		if (argc > argvsize) {
			argc = SES_ENOMEM;
		} else {
			for(int i = 0; i < p->tkn_ct; ++i) {
				uintptr_t offset = (uintptr_t)p->tkn_list[i] - (uintptr_t)p->tkn_list[0];
				if (i != 0) {
					arg[offset-1] = '\0';
				}
				argv[i] = &arg[offset];
			}
		}
		free((void*)p);
	}
	return argc;
}

static inline int ses_tokenize_strtok(char *argv[restrict], size_t argvsize, char line[restrict])
{
	int argc = 0;
	const char * delim = " \t\n";
#if __POSIX_VISIBLE >= 200809
	for(char *tok = line, *ptrptr; (tok = strtok_r(tok, delim, &ptrptr)); tok = NULL)
#else
	for(char *tok = line; (tok = strtok(tok, delim)); tok = NULL)
#endif
	{
		if (tok != line)
			tok[-1] = '\0';
		argv[argc++] = tok;
		if (argc > argvsize) {
			return SES_ENOMEM;
		}
	}
    return argc;
}

void ses_printinfo(const struct ses_cmds_s cmds[restrict], size_t cmdssize)
{
	for(; cmdssize--; ++cmds) {
#if SES_USE_CMDS_DESCRIPTIONS
		printf(" %s - %s\n", cmds[0].name, cmds[0].desc?cmds[0].desc:"");
#else
		printf(" %s\n", cmds[0].name);
#endif
	}
}

bool ses_cmds_is_duplicated(const struct ses_cmds_s cmds[restrict], size_t cmdscnt)
{
	assert(cmds && cmdscnt);
	for(;cmdscnt--; ++cmds) {
		for(int i = 1; i < cmdscnt; ++i) {
			if (!strcmp(cmds[i].name, cmds[0].name)) {
				return true;
			}
		}
	}
	return false;
}

void ses_printf_issue(void)
{
	printf("\n"
			"-----------------------------------------\n"
			" Simple Embedded Shell version " SES_VERSION "\n"
			" Type 'help' for commands list.\n"
			"-----------------------------------------\n"
			"\n");
}

void ses_printf_help(const struct ses_cmds_s cmds[restrict], size_t cmdscnt)
{
	assert(cmds && cmdscnt);
	printf(
			"Simple Embedded Shell, version " SES_VERSION " on <target>\n"
			"These shell commands are defined internally:\n"
			" help - print this text\n"
			"\n"
			"These shell commands are defined by application:\n");
	ses_printinfo(cmds, cmdscnt);
	printf("\n"
			"Written by Kamil Cukrowski 2017.\n"
			"\n");
}

int ses_tokenize(char *argv[restrict], size_t argvsize, char line[restrict])
{
	assert(argv && argvsize);
#if SES_USE_ao_string_tokenize
	return ses_tokenize_ao_string_tokenize(argv, argvsize, line);
#else
	return ses_tokenize_strtok(argv, argvsize, line);
#endif
}

int ses_system(const struct ses_cmds_s cmds[restrict], size_t cmdscnt,
		int argc, char *argv[restrict],
		int *exit_status)
{
	assert(cmds && cmdscnt && argc > 0 && argv);
	assert(!ses_cmds_is_duplicated(cmds, cmdscnt));
	for(;cmdscnt--; ++cmds) {
		if (!strcmp(cmds[0].name, argv[0])) {
			assert(cmds[0].func != NULL);
			const int ret = cmds[0].func(argc, argv);
			if (exit_status) *exit_status = ret;
			return 0;
		}
	}
	return SES_ECOMMAND_NOT_FOUND;
}

void ses_run_line(const struct ses_cmds_s cmds[restrict], size_t cmdscnt,
		char *argv[restrict], size_t argvcnt,
		char line[restrict])
{
	if (line[0] == '\0') {
		return;
	}

	if (!strcmp("help", line)) {
		ses_printf_help(cmds, cmdscnt);
		return;
	}

	const int argc = ses_tokenize(argv, argvcnt, line);
	if (argc < 1) {
		if (argc == SES_ENOMEM) {
			fprintf(stderr, "Out of memory for function arguments!\n");
			return;
		}
		fprintf(stderr, "Unhandled ses_tokenize error: %d\n", argc);
		return;
	}

	int exit_status;
	const int ret = ses_system(cmds, cmdscnt, argc, argv, &exit_status);
	if (ret != 0) {
		if (ret == SES_ECOMMAND_NOT_FOUND) {
			fprintf(stderr, "%s: Command not found.\n"
					"Type 'help' for commands list.\n", argv[0]);
			return;
		}
		fprintf(stderr, "Unhandled ses_system error: %d\n", ret);
		return;
	}
	if (exit_status != 0) {
		printf("Last command returned %d\n", exit_status);
	}
}
