/*
 * sectionconf_config.h
 *
 *  Created on: 16 kwi 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */

#ifndef SRC_LIB_SECTIONCONF_SECTIONCONF_CONFIG_H_
#define SRC_LIB_SECTIONCONF_SECTIONCONF_CONFIG_H_

#include <sectionconf/types.h>

struct sectionconf_s {
	SECTIONCONF_ADD_HEADER();

	long my_mymber;


} __attribute__((__packed__));

#endif /* SRC_LIB_SECTIONCONF_SECTIONCONF_CONFIG_H_ */
