/*
 * temperature.h
 *
 *  Created on: Sep 15, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_TEMPERATURE_H_
#define INC_MYCODE_TEMPERATURE_H_

#include "stm32g0xx_hal.h"

float getTemperature();
uint16_t getADCValue(void);

#endif /* INC_MYCODE_TEMPERATURE_H_ */
