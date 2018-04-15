/*
 * ses.c
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */
#define _POSIX_C_SOURCE 200809L

#include <ses/ses.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <time.h>

static inline int ses_tokenize_ao_string_tokenize(char **argv, size_t argvsize, char * arg)
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
		printf("Parsing string ``%s'' failed:\n\terrno %d\n", arg, errno);
		argc = -ENOMEM;
	} else {
		//printf("Parsed string ``%s''\ninto %d tokens:\n", arg, p->tkn_ct);
		argc = p->tkn_ct;
		if (argc > argvsize) {
			argc = -ENOMEM;
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

static inline int ses_tokenize_strtok(char ** restrict argv, size_t argvsize, char * restrict arg)
{
	int argc = 0;
	const char * delim = " \t\n";
#if __POSIX_VISIBLE >= 200809
	for(char *tok = arg, *ptrptr; (tok = strtok_r(tok, delim, &ptrptr)); tok = NULL)
#else
	for(char *tok = arg; (tok = strtok(tok, delim)); tok = NULL)
#endif
	{
		if (tok != arg)
			tok[-1] = '\0';
		argv[argc++] = tok;
		if (argc > argvsize) {
			return -ENOMEM;
		}
	}
    return argc;
}

void ses_printinfo(const struct ses_cmds_s cmds[restrict], size_t cmdssize)
{
	for(; cmdssize--; ++cmds) {
		printf(" %s - %s\n", cmds[0].name, cmds[0].desc?cmds[0].desc:"");
	}
}

int ses_tokenize(char ** restrict argv, size_t argvsize, char * restrict arg)
{
	assert(argv && argvsize);
#if SES_USE_ao_string_tokenize
	return ses_tokenize_ao_string_tokenize(argv, argvsize, arg);
#else
	return ses_tokenize_strtok(argv, argvsize, arg);
#endif
}

int ses_internal_help(const struct ses_cmds_s cmds[restrict], size_t cmdssize,
		int argc, char *argv[])
{
	printf(
			"Simple Embedded Shell, version 0.0.1 on <target>\n"
			"These shell commands are defined internally:\n"
			" help - print this text\n"
			"\n"
			"These shell commands are defined by application:\n");
	ses_printinfo(cmds, cmdssize);
	printf("\n"
			"Written by Kamil Cukrowski 2017.\n"
			"\n");
}

bool ses_cmds_is_duplicated(const struct ses_cmds_s cmds[restrict], size_t cmdssize)
{
	for(;cmdssize--; ++cmds) {
		for(int i = 1; i < cmdssize; ++i) {
			if (!strcmp(cmds[i].name, cmds[0].name)) {
				return true;
			}
		}
	}
	return false;
}

int ses_exec(const struct ses_cmds_s cmds[restrict], size_t cmdssize,
		char ** restrict argv, size_t argvcnt, char * restrict line)
{
	assert(!ses_cmds_is_duplicated(cmds, cmdssize));
	const int argc = ses_tokenize(argv, argvcnt, line);
	if (argc < 0) return argc;
	if (!strcmp("help", argv[0])) {
		return ses_internal_help(cmds, cmdssize, argc, argv);
	}
	for(;cmdssize--; ++cmds) {
		if (!strcmp(cmds[0].name, argv[0])) {
			assert(cmds[0].func != NULL);
			return (char)cmds[0].func(argc, argv);
		}
	}
	printf("%s: Command not found.\n", argv[0]);
	printf(" Type 'help' for commands list.\n");
	return INT_MIN;
}

void ses_display_prompt_esc_t()
{
	time_t now = time(NULL);
	int seconds = now%60;
	now /= 60;
	int minutes = now%60;
	int hours = now / 60;
	printf("%d:%d:%d", hours, minutes, seconds);
}
void ses_display_prompt_esc_h()
{
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif
	char buffer[HOST_NAME_MAX];
	(void)gethostname(buffer, sizeof(buffer));
	fwrite(buffer, 1, strlen(buffer), stdout);
}
void ses_display_prompt_esc_u()
{
	fwrite(getlogin(), 1, strlen(getlogin()), stdout);
}

void ses_display_prompt() {
	const char prompt[] = SES_PS1;
#if SES_USE_PS1_ESCAPE_SEQUENCES
	for(const char * c = &prompt[0]; *c != '\0'; ++c) {
		if (*c == '\\') {
			++c;
			assert(*c != '\0');
			switch(*c) {
			case '\\': fputc(*c, stdout); break;
			case 't': ses_display_prompt_esc_t(); break;
			case 'h': ses_display_prompt_esc_h(); break;
			case 'u': ses_display_prompt_esc_u(); break;
			default: assert(0); break;
			}
		} else {
			fputc(*c, stdout);
		}
	}
#else
	fwrite(prompt, sizeof(prompt)-1, f);
#endif
}

int ses_internal_echo(int argc, char *argv[])
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

int ses_internal_abort(int argc, char *argv[])
{
	abort();
	return 0;
}

int ses_internal_exit(int argc, char *argv[])
{
	if (argc > 2) {
		exit(atoi(argv[1]));
	} else {
		exit(0);
	}
	return 0;
}
