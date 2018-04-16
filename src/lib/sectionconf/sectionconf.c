/*
 * sectionconf.c
 *
 *  Created on: 8 mar 2018
 *      Author: kamil
 */
#include <sectionconf/sectionconf.h>
#include <endian.h>
#include <inttypes.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

__attribute__((__used__))
__attribute__((__section__(".sectionconf"),__used__))
const struct sectionconf_s sectionconf;

void sectionconf_header_init(struct sectionconf_header_s *h)
{
	assert(h);
	const uint8_t magic_start[] = SECTIONCONF_HEADER_MAGIC_START;
	memcpy(h->magic_start, magic_start, sizeof(h->magic_stop));
	const uint8_t version[] = SECTIONCONF_HEADER_VERSION;
	memcpy(h->version, version, sizeof(h->magic_stop));
	h->sectionlength = sizeof(struct sectionconf_s);
	const uint8_t magic_stop[] = SECTIONCONF_HEADER_MAGIC_STOP;
	memcpy(h->magic_stop, magic_stop, sizeof(h->magic_stop));
	assert(sectionconf_header_isValid(h) == true);
}

bool sectionconf_header_isValid(const struct sectionconf_header_s *h)
{
	assert(h);
	const uint8_t magic_start[] = SECTIONCONF_HEADER_MAGIC_START;
	static_assert(sizeof(h->magic_start) == sizeof(magic_start), "");
	if(memcmp(h->magic_start, magic_start, sizeof(h->magic_start))) return false;
	const uint8_t version[] = SECTIONCONF_HEADER_VERSION;
	static_assert(sizeof(h->version) == sizeof(version), "");
	if(memcmp(h->version, version, sizeof(h->magic_start))) return false;
	if(h->sectionlength != sizeof(struct sectionconf_s)) return false;
	const uint8_t magic_stop[] = SECTIONCONF_HEADER_MAGIC_STOP;
	static_assert(sizeof(h->magic_stop) == sizeof(magic_stop), "");
	if(memcmp(h->magic_stop, magic_stop, sizeof(h->magic_stop))) return false;
	return true;
}

__weak_symbol
bool sectionconf_isValid(const struct sectionconf_s *s)
{
	static_assert(CHAR_BIT == 8, "");
	static_assert(sizeof(bool) == sizeof(uint8_t), "");
	if(!sectionconf_header_isValid(&s->sectionconf_header)) return false;
	return true;
}

__weak_symbol
void sectionconf_printf(const struct sectionconf_s *s)
{
}
