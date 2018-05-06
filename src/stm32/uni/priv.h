/*
 * priv.h
 *
 *  Created on: 5 maj 2018
 *      Author: kamil
 */

#ifndef SRC_KAMILLIBCSTM32_UNI_PRIV_H_
#define SRC_KAMILLIBCSTM32_UNI_PRIV_H_

// should be exported from stm32cubeMX generated code in place of main with endless loop removed
void MX_Init(void);
// used to save watchdog state before resetting watchdog flag
void uni_wdg_init(void);

#endif /* SRC_KAMILLIBCSTM32_UNI_PRIV_H_ */
