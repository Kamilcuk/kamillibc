/*
 * ses.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SES_H_
#define SRC_APPLICATION_SES_H_

#include <ses/ses_config.h>
#include <stdio.h>
#include <stdbool.h>

struct ses_cmds_s {
	const char * name;
	union {
		int (*func)(int argc, char *argv[]);
		int (*vfunc)(void);
	};
	const char * desc;
};

void ses_printinfo(const struct ses_cmds_s cmds[restrict], size_t size);
int ses_tokenize(char ** restrict argv, size_t argvsize, char * restrict arg);
int ses_internal_help(const struct ses_cmds_s cmds[restrict], size_t cmdssize,
		int argc, char *argv[]);
bool ses_cmds_is_duplicated(const struct ses_cmds_s cmds[restrict], size_t cmdssize);
int ses_exec(const struct ses_cmds_s cmds[restrict], size_t cmdssize,
		char ** restrict argv, size_t argvcnt, char * restrict line);

void ses_display_prompt();

#define SES_CMDS_ADD_INTERNAL(name)  { #name, &ses_internal_##name }
int ses_internal_echo(int argc, char *argv[]);
#define SES_CMDS_ADD_INTERNAL_ECHO() SES_CMDS_ADD_INTERNAL(echo)
int ses_internal_abort(int argc, char *argv[]);
#define SES_CMDS_ADD_INTERNAL_ABORT() SES_CMDS_ADD_INTERNAL(abort)
int ses_internal_exit(int argc, char *argv[]);
#define SES_CMDS_ADD_INTERNAL_EXIT() SES_CMDS_ADD_INTERNAL(exit)

#define SES_CMDS_ADD_INTERNAL_ALL() \
	SES_CMDS_ADD_INTERNAL_ECHO(), \
	SES_CMDS_ADD_INTERNAL_ABORT(), \
	SES_CMDS_ADD_INTERNAL_EXIT()

#endif /* SRC_APPLICATION_SES_H_ */
