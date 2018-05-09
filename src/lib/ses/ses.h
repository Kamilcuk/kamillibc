/*
 * ses.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SES_H_
#define SRC_APPLICATION_SES_H_

#include <ses/ses_config.h>
#include <ses/builtin.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define SES_VERSION "0.0.1"

enum {
	SES_OK = 0,
	SES_ENOMEM,
	SES_ECOMMAND_NOT_FOUND,
};

struct ses_cmds_s {
	const char * name;
	int (*func)(int argc, char *argv[]);
#if SES_USE_CMDS_DESCRIPTIONS
	const char * desc;
#endif
};

void ses_printinfo(const struct ses_cmds_s cmds[restrict], size_t size);
bool ses_cmds_is_duplicated(const struct ses_cmds_s cmds[restrict], size_t cmdscnt);

void ses_printf_issue(void);
void ses_printf_help(const struct ses_cmds_s cmds[restrict], size_t cmdscnt);
int ses_tokenize(char *argv[restrict], size_t argvsize, char line[restrict]);
int ses_system(const struct ses_cmds_s cmds[restrict], size_t cmdscnt,
		int argc, char *argv[restrict],
		int *exit_status);

void ses_run_line(const struct ses_cmds_s cmds[restrict], size_t cmdscnt,
		char *argv[restrict], size_t argvcnt,
		char line[restrict]);

#endif /* SRC_APPLICATION_SES_H_ */
