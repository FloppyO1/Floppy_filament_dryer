/*
 * settings.c
 *
 *  Created on: Jul 14, 2024
 *      Author: Filippo Castellan
 */

#include "MyCode/settings.h"
#include <stdint.h>
#include <string.h>

// DEFINEs & OBJECTs & VARIABLES FOR EEPROM
#define EEPROM_ADDRESS 0xA0	//devide address
extern I2C_HandleTypeDef hi2c1;

void storeSettingsToEeprom(float kp, float ki, float kd) {
	uint8_t data[4];
	// write to memory kp
	floatToBytes(kp, data);
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		write_byte_eeprom(i, data[i]);
	}
	// write to memory ki
	floatToBytes(ki, data);
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		write_byte_eeprom(i + sizeof(float), data[i]);
	}
	// write to memory kd
	floatToBytes(kd, data);
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		write_byte_eeprom(i + (2 * sizeof(float)), data[i]);
	}
}

void loadSettingsFromEeprom(float *kp, float *ki, float *kd) {
	uint8_t data[4];
	// read to memory kp
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		data[i] = read_byte_eeprom(i);
	}
	*kp = bytesToFloat(data);
	// read to memory ki
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		data[i] = read_byte_eeprom(i + sizeof(float));
	}
	*ki = bytesToFloat(data);
	// read to memory kd
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < sizeof(float); i++) {
		data[i] = read_byte_eeprom(i + (2 * sizeof(float)));
	}
	*kd = bytesToFloat(data);

}

void floatToBytes(float value, uint8_t *array) {
	memcpy(array, &value, sizeof(float));
}

float bytesToFloat(const uint8_t *array) {
	float value;
	memcpy(&value, array, sizeof(float));
	return value;
}

uint8_t read_byte_eeprom(uint8_t address) {
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	return data;
}

void write_byte_eeprom(uint8_t address, uint8_t value) {
	uint8_t data = value;
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	HAL_Delay(10);
}
