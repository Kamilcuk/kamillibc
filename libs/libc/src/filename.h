/*
 * filename.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FILENAME_H_
#define SRC_FILENAME_H_

#include <string.h>

#define FILENAME  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#endif /* SRC_FILENAME_H_ */
