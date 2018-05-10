/*
 * ses_config.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SES_CONFIG_H_
#define SRC_APPLICATION_SES_CONFIG_H_

#ifndef SES_USE_ao_string_tokenize
#define SES_USE_ao_string_tokenize 0
#endif
#if SES_USE_ao_string_tokenize
#include "tokenize/ao_string_tokenize.h"
#endif

#ifndef SES_USE_CMDS_DESCRIPTION
#define SES_USE_CMDS_DESCRIPTION 0
#endif

#endif /* SRC_APPLICATION_SES_CONFIG_H_ */
