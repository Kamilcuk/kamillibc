/*
 * pwm.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_UNI_PWM_H_
#define SRC_LIB_UNI_PWM_H_

#include <uni/types.h>

#ifndef __pwm_t_defined
typedef int pwm_t;
#define __pwm_t_defined 1
#endif

void pwm_set_dutycycle(pwm_t t, float dutycycle);
void pwm_set_frequency(pwm_t t, unsigned int frequency);
void pwm_set_period(pwm_t t, unsigned int period);

#endif /* SRC_LIB_UNI_PWM_H_ */
