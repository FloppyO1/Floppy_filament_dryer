/*
 * pid.c
 *
 *  Created on: Jul 12, 2025
 *      Author: Filippo Castellan
 */

#include <stdio.h>
#include "tim.h"
#include "MyCode/pid.h"
#include "Libraries/temperature.h"
#include "MyCode/display.h"
#include "Libraries/Display/ssd1306.h"
#include "MyCode/config.h"

extern volatile uint8_t currentMenu;
extern uint32_t timerTimer;

void setDuty(uint8_t duty) {
	TIM17->CCR1 = duty;
}

uint8_t pid_control(float kp, float ki, float kd, float target, float real, uint16_t dutymax) {
	static float integral = 0.0f;
	static float previous_error = 0.0f;
	const float dt = 0.5f; // interval of (500 ms)

	float error = target - real;
	integral += error * dt;
	float derivative = (error - previous_error) / dt;
	previous_error = error;
//	if(error >= 10) {	// for a more rapid response and minor oscillation
//		kp *= 3;
//	}
//	if (error >= 10) {// to keep the overshoot small and have long term error reduced
//		integral = 0.0f;	// reset the integral value if the error is more than 10 deg
//	}

	float output = kp * error + ki * integral + kd * derivative;

	if (output > dutymax) {	// do not increment the integral term if the duty is already at max value. To prevent integral wind-up
		integral -= error * dt;
		output = kp * error + ki * integral + kd * derivative;
	}

// convert the range of the duty obtained
	if (output < 0)
		output = 0;
	if (output > dutymax)
		output = dutymax;

// for serial debug
	uint8_t str[100];
	sprintf(str, "kpe: %0.2f\tkie: %0.2f\tkde: %0.2f\tout: %0.2f\n", kp * error, ki * integral, kd * error, output);
	printString(str);

	return (uint8_t) output;
}

/*
 * THIS IS A BLOCKING FUNCTION, ONLY AT THE END OF IT THE PROGRAM EXECUTION CAN CONTINUE
 */
void PIDautoCal(float tempTarget, float hysteresis, uint8_t cycles, float *kp, float *ki, float *kd) { // bang-bang Ziegler-Nicloas PID auto calibration
	uint8_t str[50];
	printString("START PID AUTOTUNE PROCESS...\n");

	uint32_t timer1 = HAL_GetTick(); // for 'millis()' non blocking application during the steps loop
	uint16_t temps[MAX_READINGS_NUMBER];	// used for storing temp measurements. With one measure every seconds it can hold ~16min should be enough
	uint16_t nextTempsIndex = 0;

	/*	STEP 1
	 *  Make the system oscillating on its natural frequency.
	 *  1. Heating until Treal > Ttarget + hysteresis
	 *  2. Let the system cool down until reach Treal < Ttarget - hysteresis
	 *  3. Repeat from the point n 1 for 'repetitions' times
	 *  During this steps the temperature value are stored for the future max, min and period calculations
	 */
	uint8_t cycleNumber = 0;
	uint8_t turnedOn = 0;
	while (1) {
		// heating controll
		float Treal = getTemperature();
		HAL_GPIO_WritePin(LED_HEATING_GPIO_Port, LED_HEATING_Pin, turnedOn);
		if (Treal < tempTarget - hysteresis && !turnedOn) {	// turn on the heater
			setDuty(UINT8_MAX);
			cycleNumber++;	// start next cycle
			turnedOn = 1;
			// serial debug
			sprintf(str, "New cycle began, cycle number %d\n", cycleNumber);
			printString(str);
		}
		if (Treal > tempTarget + hysteresis && turnedOn) {	// turn off the heater
			setDuty(0);
			turnedOn = 0;
		}

		if (HAL_GetTick() - timer1 >= READINGS_PERIOD) { // repeat what's inside every 2 second
			// reset the timer
			timer1 = HAL_GetTick();
			// read temp and store into the array
			storeNewTemp(Treal, &nextTempsIndex, temps);
			// serial debug
			sprintf(str, "Ttarget= %f\tTreal= %f\n", tempTarget, getStoredTemp(temps, nextTempsIndex - 1));
			printString(str);
			// display state
			ssd1306_Fill(Black);
			sprintf(str, "Ts: %0.0f", tempTarget);
			DISPLAY_PrintStrMedium(str, 0, 0);
			sprintf(str, "Tr %0.2f", getStoredTemp(temps, nextTempsIndex - 1));
			DISPLAY_PrintStrMedium(str, 0, 11);
			sprintf(str, "Cy: %d/%d", cycleNumber, cycles);
			DISPLAY_PrintStrMedium(str, 0, 22);
			// dislay graph
			ssd1306_Line(59, 30, 127, 30, White);	// x axis
			ssd1306_Line(60, 0, 60, 32, White);	// y axis
			int limit = 0;
			if (nextTempsIndex - 1 >= 67)
				limit = 67;
			else
				limit = nextTempsIndex - 1;
			// find max and min value
			float absMax = 0;
			float absMin = 20000.0f;
			if (limit >= 2) {
				for (int x = 1; x < limit; x++) {
					if (getStoredTemp(temps, nextTempsIndex - x) > absMax)
						absMax = getStoredTemp(temps, nextTempsIndex - x);
					if (getStoredTemp(temps, nextTempsIndex - x) < absMin)
						absMin = getStoredTemp(temps, nextTempsIndex - x);
				}
				float scale = 30.0f / (2.0f + absMax - absMin);	// the scale vary according to the max and min value to display +2 for some margin
				//sprintf(str, "Graph: scale %f\t max %f\t min %f\n", scale, getStoredTemp(temps, absMaxIndex), getStoredTemp(temps, absMinIndex));
				printString(str);
				for (int x = 2; x < limit; x++) {	// take the last 50 value of temp
					uint8_t yPrev = (uint8_t) (30.0f - ((getStoredTemp(temps, nextTempsIndex - x - 1) - absMin) * scale));
					uint8_t yNow = (uint8_t) (30.0f - ((getStoredTemp(temps, nextTempsIndex - x) - absMin) * scale));
					ssd1306_Line(128 - x - 1, yPrev, 128 - x, yNow, White);
					//ssd1306_DrawPixel(128 - x, (uint8_t) (30.0f - (getStoredTemp(temps, nextTempsIndex - x) - getStoredTemp(temps, absMinIndex)) * scale), White);
				}
			}

			ssd1306_UpdateScreen();
		}
		HAL_GPIO_TogglePin(LED_ALIVE_GPIO_Port, LED_ALIVE_Pin);
		if (cycleNumber >= cycles + 1) {	// the first step is finished
			setDuty(0);	// turn of the heater
			break;
		}
	}

	// serial debug
	sprintf(str, "Measurements made %d\n", nextTempsIndex - 1);
	printString(str);

	/* STEP 2
	 * Analysis of all data collected:
	 * - avgMax: highest point of the oscillation (first 2 cycles not considered)
	 * - avgMin: lowest point of the oscillation (first 2 cycle not considered)
	 * - Pu: period of the oscillation
	 *
	 * 	  max         max         max
	 * °C  _           _           _
	 *  | / \         / \         / \
	 *  |/   \       /   \       /   \
	 *  +-----\-----/-----\-----/-----\-------time
	 *  |      \   /       \   /       \
	 *  |       \_/         \_/
	 *          min         min
	 * 	   |----Pu-----|----Pu-----|
	 *
	 */
	uint16_t maxsIndex[20];
	uint16_t nextMaxsIndex = 0;
	uint16_t minsIndex[20];
	uint16_t nextMinsIndex = 0;
	/* FIND ALL THE MAXs AND  MINs
	 *  MAXs: check a value, if the previous two values are lower or equal, check the following two, if they are lower or equal, the checked value is a max
	 *	same applied for the min search but inverted (greater or equal)
	 *
	 * prev1  prev2  check  foll1  foll2
	 *   |		|	   |      |      |
	 *   V      V      V      V      V
	 * [20.5,  22.5,  22.7,  22.7,  22.4]
	 */
	for (int i = MAX_MIN_FINDER_WINDOW; i < nextTempsIndex - 1 - MAX_MIN_FINDER_WINDOW; i++) {
		uint16_t isMax = 1;
		uint16_t isMin = 1;
		for (int j = 1; j <= MAX_MIN_FINDER_WINDOW; j++) {
			if (temps[i] <= temps[i - j]) {
				isMax = 0;
				break;
			}
			if (temps[i] < temps[i + j]) {
				isMax = 0;
				break;
			}
		}
		for (int j = 1; j <= MAX_MIN_FINDER_WINDOW; j++) {
			if (temps[i] >= temps[i - j]) {
				isMin = 0;
				break;
			}
			if (temps[i] > temps[i + j]) {
				isMin = 0;
				break;
			}
		}
		if (isMax) {
			maxsIndex[nextMaxsIndex] = i;
			// serial debug
			sprintf(str, "Max found at time %d seconds, with value %0.2f°C\n", i, getStoredTemp(temps, i));
			printString(str);
			nextMaxsIndex++;
		}
		if (isMin) {
			minsIndex[nextMinsIndex] = i;
			// serial debug
			sprintf(str, "Min found at time %d seconds, with value %0.2f°C\n", i, getStoredTemp(temps, i));
			printString(str);
			nextMinsIndex++;
		}
	}
	/*
	 * CALCULATING THE EVERAGE MIN AND MAX
	 * first the value are validated
	 */
	// serial debug
	printString("Calcualting min and max values\n");
	float avgMax = 0.0f;
	float avgMin = 0.0f;
	for (int i = INITIAL_OSCILLATION_TO_SKIP; i < nextMaxsIndex - 1; i++) {	// -1 for the first skipped, -1 for the correct number (nextMinsIndex are one more)
		avgMax += getStoredTemp(temps, maxsIndex[i]);
	}
	for (int i = INITIAL_OSCILLATION_TO_SKIP; i < nextMinsIndex - 1; i++) {	// -1 for the first skipped, -1 for the correct number (nextMinsIndex are one more)
		avgMin += getStoredTemp(temps, minsIndex[i]);
	}
	avgMax /= nextMaxsIndex - 1 - INITIAL_OSCILLATION_TO_SKIP;
	avgMin /= nextMinsIndex - 1 - INITIAL_OSCILLATION_TO_SKIP;
	// serial debug
	sprintf(str, "Average max is %0.2f, average min is %0.2f\n", avgMax, avgMin);
	printString(str);

	/*
	 * CALCULATE KU AND PU
	 * Ku is the critical gain of the system
	 * Pu is the period of the oscillations
	 * D is half of the difference of the min and max signal given to the system ((255-0)/2 in our case) !!!MUST HAVE THE SAME RAGE OF THE FINAL PID CONTROLLER!!!
	 * A is half of the difference between the avgMax and avgMin
	 */
	// serial debug
	printString("Start the parameter calculation, last step\n");
	float ku = 0.0f;
	float Pu = 0.0f;
	const float D = UINT8_MAX / 2.0f; 	// must be
	float A = (avgMax - avgMin) / 2.0f;
	ku = (4 * D) / (3.14159265359 * A);
	for (int i = INITIAL_OSCILLATION_TO_SKIP; i < nextMaxsIndex - 1 - INITIAL_OSCILLATION_TO_SKIP; i++) {    // skip some peak
		Pu += maxsIndex[i] * READINGS_PERIOD / 1000; // * second between each reading
	}
	Pu = Pu / (nextMaxsIndex - 1 - INITIAL_OSCILLATION_TO_SKIP);  // -1 for the first skipped, -1 for the correct number (nextMinsIndex are one more)
	// serial debug
	sprintf(str, "Ku is %f, Pu is %f, D is %f, A is %f\n", ku, Pu, D, A);
	printString(str);

	// display state
	ssd1306_Fill(Black);
	sprintf(str, "Average max: %0.2f", avgMax);
	DISPLAY_PrintStrMedium(str, 0, 0);
	sprintf(str, "Average min: %0.2f", avgMin);
	DISPLAY_PrintStrMedium(str, 0, 11);
	sprintf(str, "Oscil. period: %0.2f", Pu);
	DISPLAY_PrintStrMedium(str, 0, 22);
	ssd1306_UpdateScreen();
	HAL_Delay(3000);

	// display state
	ssd1306_Fill(Black);
	sprintf(str, "Ku: %0.2f", ku);
	DISPLAY_PrintStrMedium(str, 0, 0);
	sprintf(str, "D: 1.0f");
	DISPLAY_PrintStrMedium(str, 0, 11);
	sprintf(str, "A: %0.2f", A);
	DISPLAY_PrintStrMedium(str, 0, 22);
	ssd1306_UpdateScreen();
	HAL_Delay(3000);

	/* STEP 3
	 * Calculate the final Kp, Ki, Kd parameters
	 */
	// NO OVERSHOOT VERSION
//	*kp = 0.2f * ku;
//	*ki = (0.4f * ku) / Pu;
//	*kd = 0.066 * ku * Pu;
	// CLASSIC VERSION
//	*kp = 0.6f * ku;
//	*ki = (2.0f * *kp) / Pu;
//	*kd = (*kp * Pu) / 8.0f;
	// PI VERSION (NO DERIVATIVE)  !! MODIFIED !!
	*kp = 0.45f * ku;
	*ki = (1.5f * *kp) / (Pu / 8);	// modified the period / 8 to have an high ki term (otherwise an offset don't go away)
	*kd = 0;

	// serial debug
	sprintf(str, "Kp is %f, Ki is %f, Kd is %f,\n", *kp, *ki, *kd);
	printString(str);

	timerTimer = HAL_GetTick();
	currentMenu = MENU_AUTOCAL_2;
}

void storeNewTemp(float temp, uint16_t *index, uint16_t *temps) {
	temps[*index] = (uint16_t) (temp * 100.0f);
	*index += 1;
}

float getStoredTemp(uint16_t *temps, uint16_t index) {
	return ((float) temps[index]) / 100.0f;
}
