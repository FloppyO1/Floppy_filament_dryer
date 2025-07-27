/*
 * temperature.c
 *
 *  Created on: Sep 15, 2024
 *      Author: Filippo Castellan
 */

#include "Libraries/temperature.h"
#include "MyCode/config.h"
#include <math.h>

extern ADC_HandleTypeDef hadc1;

float getTemperature() {
	float r = RESOLUTION / getADCValue() - 1;
	r = RBALANCE / r;

	float t = r / NTC_NOMINAL;
	t = log(t);
	t /= BETA;
	t += 1.0 / (25 + 273.15);
	t = 1.0 / t;
	t -= 273.15;
	return t;
}

uint16_t getADCValue(void){
	uint32_t adc = 0;
	const uint8_t conversionNumber = 100;
	for(int i = 0; i<conversionNumber; i++){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		HAL_ADC_Stop(&hadc1);
		adc += HAL_ADC_GetValue(&hadc1);
	}

	return adc/conversionNumber;
}

