/*
 * types.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_TYPES_H_
#define SRC_LIB_TIME_EX_TYPES_H_

#include <time.h>

#ifndef __time_day_t_defined
typedef time_t time_day_t;
#define __time_day_t_defined 1
#endif

#ifndef __time_hour_t_defined
typedef time_t time_hour_t;
#define __time_hour_t_defined 1
#endif

#ifndef __time_ms_t_defined
typedef time_t time_ms_t;
#define __time_ms_t_defined 1
#endif

#ifndef __time_us_t_defined
typedef time_t time_us_t;
#define __time_us_t_defined 1
#endif

#ifndef __time_ns_t_defined
typedef time_t time_ns_t;
#define __time_ns_t_defined 1
#endif

#ifndef __time_ps_t_defined
typedef time_t time_ps_t;
#define __time_ps_t_defined 1
#endif

#endif /* SRC_LIB_TIME_EX_TYPES_H_ */
