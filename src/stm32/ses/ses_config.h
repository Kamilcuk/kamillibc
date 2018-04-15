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

// PS1 – Default interaction prompt
#ifndef SES_PS1
#define SES_PS1 " uptime: \\t > "
#endif

#ifndef SES_USE_PS1_ESCAPE_SEQUENCES
#define SES_USE_PS1_ESCAPE_SEQUENCES 1
#endif

#endif /* SRC_APPLICATION_SES_CONFIG_H_ */
