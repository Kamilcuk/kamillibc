/*
 * builtin.h
 *
 *  Created on: 17 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_SES_BUILTIN_H_
#define SRC_STM32_SES_BUILTIN_H_

#include <ses/ses_config.h>

#if SES_USE_CMDS_DESCRIPTION
#define SES_CMDS_ADD_BUILTIN(name, ...)  { #name, &ses_builtin_##name, ##__VA_ARGS__ }
#else
#define SES_CMDS_ADD_BUILTIN(name, ...)  { #name, &ses_builtin_##name }
#endif

int ses_builtin_echo(int argc, char *argv[]);
#define SES_CMDS_ADD_BUILTIN_ECHO() SES_CMDS_ADD_BUILTIN(echo, "display a line of text")

int ses_builtin_abort(int argc, char *argv[]);
#define SES_CMDS_ADD_BUILTIN_ABORT() SES_CMDS_ADD_BUILTIN(abort, "Calls abort()")

int ses_builtin_exit(int argc, char *argv[]);
#define SES_CMDS_ADD_BUILTIN_EXIT() SES_CMDS_ADD_BUILTIN(exit, "Calls exit()")

int ses_builtin_testargs(int argc, char *argv[]);
#define SES_CMDS_ADD_BUILTIN_testargs() SES_CMDS_ADD_BUILTIN(testargs, "Tests command arguments")


#define SES_CMDS_ADD_BUILTIN_ALL() \
	SES_CMDS_ADD_BUILTIN_ECHO(), \
	SES_CMDS_ADD_BUILTIN_ABORT(), \
	SES_CMDS_ADD_BUILTIN_EXIT()


#endif /* SRC_STM32_SES_BUILTIN_H_ */
