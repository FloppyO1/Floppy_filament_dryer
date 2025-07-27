/*
 * settings.h
 *
 *  Created on: Jul 14, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include "stm32g0xx_hal.h"

#define EEPROM_DATA_BEGIN_ADDRESS 0	// data address
#define EEPROM_ISFIRSTBOOT_ADDRESS 100	// isfirstboot data address

void loadSettingsFromEeprom(float *kp, float *ki, float *kd);
void storeSettingsToEeprom(float kp, float ki, float kd);
void floatToBytes(float value, uint8_t *array);
float bytesToFloat(const uint8_t *array);
uint8_t read_byte_eeprom(uint8_t address);
void write_byte_eeprom(uint8_t address, uint8_t value);


#endif /* INC_SETTINGS_H_ */
