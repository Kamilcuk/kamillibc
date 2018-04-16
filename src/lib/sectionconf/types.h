/*
 * types.h
 *
 *  Created on: 16 kwi 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */

#ifndef SRC_LIB_SECTIONCONF_TYPES_H_
#define SRC_LIB_SECTIONCONF_TYPES_H_

#include <uni/cdefs.h>
#include <stdint.h>

#define SECTIONCONF_HEADER_MAGIC_START { 0xAA, 0xBB, 0xCC, 0xDD }
#define SECTIONCONF_HEADER_VERSION     { 0x00, 0x01, 0x00, 0x01 }
#define SECTIONCONF_HEADER_MAGIC_STOP  { 0xDD, 0xCC, 0xBB, 0xAA }

struct sectionconf_header_s {
	uint8_t magic_start[sizeof((uint8_t[])SECTIONCONF_HEADER_MAGIC_START)];
	uint8_t version[sizeof((uint8_t[])SECTIONCONF_HEADER_VERSION)];
	uint32_t sectionlength; // little endian, equal to sizeof(struct sectionconf_s)
	uint8_t magic_stop[sizeof((uint8_t[])SECTIONCONF_HEADER_MAGIC_STOP)];
} __packed;

#define SECTIONCONF_ADD_HEADER() \
		struct sectionconf_header_s sectionconf_header

#endif /* SRC_LIB_SECTIONCONF_TYPES_H_ */
