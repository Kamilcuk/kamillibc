/*
 * sectionconf.h
 *
 *  Created on: 8 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SECTIONCONF_H_
#define SRC_APPLICATION_SECTIONCONF_H_

#include <sectionconf/types.h>
#include <sectionconf/sectionconf_config.h>
#include <stdint.h>
#include <stdbool.h>

extern const struct sectionconf_s sectionconf;

void sectionconf_header_init(struct sectionconf_header_s *h);
bool sectionconf_header_isValid(const struct sectionconf_header_s *h);
bool sectionconf_isValid(const struct sectionconf_s *s);
void sectionconf_printf(const struct sectionconf_s *s);

#endif /* SRC_APPLICATION_SECTIONCONF_H_ */
