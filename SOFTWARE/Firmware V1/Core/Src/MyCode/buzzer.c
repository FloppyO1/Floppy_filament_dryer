/*
 * buzzer.c
 *
 *  Created on: Jul 12, 2025
 *      Author: Filippo Castellan
 */

#include "MyCode/buzzer.h"
#include "tim.h"
#include "MyCode/config.h"

void BUZZER_shortPressBeep() {
	TIM16->PSC = 16 - 1;
	TIM16->CCR1 = TIM16->ARR/100*LOUDNESS_PERCENTAGE;
	HAL_Delay(50);
	TIM16->CCR1 = 0;
}

void BUZZER_longPressBeep() {
	TIM16->PSC = 16 - 1;
	TIM16->CCR1 = TIM16->ARR/100*LOUDNESS_PERCENTAGE;
	HAL_Delay(50);
	TIM16->CCR1 = 0;

	TIM16->PSC = 32 - 1;
	TIM16->CCR1 = TIM16->ARR/100*LOUDNESS_PERCENTAGE;
	HAL_Delay(50);
	TIM16->CCR1 = 0;

	TIM16->PSC = 16 - 1;
	TIM16->CCR1 = TIM16->ARR/100*LOUDNESS_PERCENTAGE;
	HAL_Delay(50);
	TIM16->CCR1 = 0;
}
