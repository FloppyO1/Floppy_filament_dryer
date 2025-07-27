/*
 * pid.h
 *
 *  Created on: Jul 12, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_PID_H_
#define INC_MYCODE_PID_H_
#include "stm32g0xx_hal.h"

void setDuty(uint8_t duty);
uint8_t pid_control(float kp, float ki, float kd, float target, float real, uint16_t dutymax);

void PIDautoCal(float tempTarget, float hysteresis, uint8_t cycles, float *kp, float *ki, float *kd);
void storeNewTemp(float temp, uint16_t *index, uint16_t *temps);
float getStoredTemp(uint16_t *temps, uint16_t index);


//void autotune_step(float setpoint, uint8_t stepDuty, uint32_t sampleTimeMs, float *kp, float *ki, float *kd);
//void updateAutocalStatus(uint8_t stepNumber, float realTemp, float setpoint, uint32_t elapsedMs);

#endif /* INC_MYCODE_PID_H_ */
