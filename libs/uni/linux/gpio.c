/*
 * gpio.c
 *
 *  Created on: 30 mar 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */
#include <uni/gpio.h>
#include <curb.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/* cppcheck-suppress useClosedFile */
/* cppcheck-suppress resourceLeak */

static void swrite(const char path[], const char data[], size_t datasize)
{
	FILE * f = fopen(path, "w");
	curb_exit(f != NULL);
	curb_exit(datasize > 0);
	curb_exit(fwrite(data, sizeof(char), datasize, f) == datasize);
	curb_exit(fclose(f) == 0);
}

void gpio_set(gpio_t t, bool value)
{
	char path[] = "/sys/class/gpio/gpio65536/direction";
	int pathlen;
	char val[] = "65536\n";
	int vallen;
	curb_exit(t < 65536);

	vallen = snprintf(val, sizeof(val), "%d\n", t);
	curb_exit(vallen > 0 && vallen < sizeof(val));
	swrite("/sys/class/gpio/export", val, vallen);

	pathlen = snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", t);
	curb_exit(pathlen > 0 && pathlen < sizeof(val));
	swrite(path, "out\n", sizeof("out\n")-1);

	pathlen = snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", t);
	curb_exit(pathlen > 0 && pathlen < sizeof(val));
	val[0] = value ? '1' : '0';
	val[2] = '\n';
	swrite(path, val, 2);
}

bool gpio_get(gpio_t t)
{
	char path[] = "/sys/class/gpio/gpio65536/direction";
	int pathlen;
	char val[] = "65536\n";
	int vallen;
	curb_exit(t < 65536);

	vallen = snprintf(val, sizeof(val), "%d\n", t);
	curb_exit( vallen > 0 && vallen < sizeof(val) );
	swrite("/sys/class/gpio/export", val, vallen);

	pathlen = snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", t);
	curb_exit( pathlen > 0 && pathlen < sizeof(val) );
	swrite(path, "in\n", sizeof("in\n")-1);

	pathlen = snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", t);
	curb_exit( pathlen > 0 && pathlen < sizeof(val) );
	FILE * f = fopen(path, "r");
	curb_exit(f != NULL);
	curb_exit((vallen = fread(val, sizeof(char), sizeof(val)/sizeof(char), f)) > 0);
	curb_exit(fclose(f) == 0);

	const long int ret = strtol(val, NULL, 10);
	curb_exit( ret != LONG_MIN && ret != LONG_MAX );
	curb_exit( ret == 0 || ret == 1 );
	return ret ? true : false;
}

