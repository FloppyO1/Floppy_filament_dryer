/*
 * buttons.c
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#include "stm32g0xx_hal.h"
#include "gpio.h"

#include "MyCode/buttons.h"
#include "MyCode/config.h"
#include "MyCode/display.h"
#include "MyCode/buzzer.h"
#include "MyCode/settings.h"

volatile BUTTON btn[3];
uint32_t lastFall = 0;

/* GLOBALS FOR SETTINGS CHANGE */
extern volatile uint8_t currentMenu;
extern volatile uint8_t currentMaterialID;
extern volatile uint16_t minutesLeft;
extern volatile float kp;
extern volatile float ki;
extern volatile float kd;

void initButtons() {
	btn[0].id = BUTTON1_Pin;
	btn[1].id = BUTTON2_Pin;
	btn[2].id = BUTTON3_Pin;

	for (int i = 0; i < 3; i++) {
		btn[i].t1 = 0;
		btn[i].pressType = NONE;
	}
}

uint8_t getButtonState(uint16_t id) {
	uint8_t state = NONE;
	for (int i = 0; i < 3; i++) {
		if (btn[i].id == id) { // check if is the button we want to read the state
			state = btn[i].pressType;
			break;
		}
	}
	return state;
}

uint8_t BUTTONS_tick() {
	uint8_t longPresRelase = 0;
	for (int i = 0; i < 3; i++) {
		if (btn[i].pressed) {
			if (HAL_GetTick() - btn[i].t1 > LONG_PRESS_TIME) {
				longPresRelase = LONG_PRESS;
			}
		}
	}

	if (getButtonState(BUTTON1_Pin) == LONG_PRESS)
		BUTTONS_longPressAction(BUTTON1_Pin);
	if (getButtonState(BUTTON2_Pin) == LONG_PRESS)
		BUTTONS_longPressAction(BUTTON2_Pin);
	if (getButtonState(BUTTON3_Pin) == LONG_PRESS)
		BUTTONS_longPressAction(BUTTON3_Pin);
	if (getButtonState(BUTTON1_Pin) == SHORT_PRESS)
		BUTTONS_shortPressAction(BUTTON1_Pin);
	if (getButtonState(BUTTON2_Pin) == SHORT_PRESS)
		BUTTONS_shortPressAction(BUTTON2_Pin);
	if (getButtonState(BUTTON3_Pin) == SHORT_PRESS)
		BUTTONS_shortPressAction(BUTTON3_Pin);
	return longPresRelase;
}

void BUTTONS_resetButtonState(uint16_t id) {
	for (int i = 0; i < 3; i++) {
		if (btn[i].id == id) {
			btn[i].pressType = NONE;
			break;
		}
	}
}

/* BUTTONS ACTION */
void BUTTONS_shortPressAction(uint16_t id) {
	/* RESET BUTTON STATE (because event served)*/
	BUTTONS_resetButtonState(id);

	switch (currentMenu) {
	case MENU_HOME:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PRESETS;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_TIMER:
		switch (id) {
		case BUTTON1_Pin:
			if (minutesLeft - MINUTES_STEP >= MINUTES_STEP)
				minutesLeft -= MINUTES_STEP;
			else
				minutesLeft = MINUTES_STEP;
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:
			if (minutesLeft + MINUTES_STEP <= MAXIMUM_HOURS * 60)
				minutesLeft += MINUTES_STEP;
			else
				minutesLeft = MAXIMUM_HOURS * 60;
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_PRESETS:
		switch (id) {
		case BUTTON1_Pin:
			currentMaterialID = MATERIAL_getPrevMaterial(currentMaterialID);
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_EDIT_PRESET1;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:
			currentMaterialID = MATERIAL_getNextMaterial(currentMaterialID);
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_EDIT_PRESET1:
		switch (id) {
		case BUTTON1_Pin:
			uint8_t temp = MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_1);
			if (temp - TEMPERATURE_STEP >= MINIMUM_TEMPERATURE)
				temp -= TEMPERATURE_STEP;
			else
				temp = MINIMUM_TEMPERATURE;
			MATERIAL_setTargetTemp(MATERIAL_ID_CUS_1, temp);
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_EDIT_PRESET2;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:
			temp = MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_1);
			if (temp + TEMPERATURE_STEP <= MAXIMUM_TEMPERATURE)
				temp += TEMPERATURE_STEP;
			else
				temp = MAXIMUM_TEMPERATURE;
			MATERIAL_setTargetTemp(MATERIAL_ID_CUS_1, temp);
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_EDIT_PRESET2:
		switch (id) {
		case BUTTON1_Pin:
			uint8_t temp = MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_2);
			if (temp - TEMPERATURE_STEP >= MINIMUM_TEMPERATURE)
				temp -= TEMPERATURE_STEP;
			else
				temp = MINIMUM_TEMPERATURE;
			MATERIAL_setTargetTemp(MATERIAL_ID_CUS_2, temp);
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_AUTOCAL_0;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:
			temp = MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_2);
			if (temp + TEMPERATURE_STEP <= MAXIMUM_TEMPERATURE)
				temp += TEMPERATURE_STEP;
			else
				temp = MAXIMUM_TEMPERATURE;
			MATERIAL_setTargetTemp(MATERIAL_ID_CUS_2, temp);
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_AUTOCAL_0:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PIDS;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_AUTOCAL_1:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			// to stop the pid autocal
			currentMenu = MENU_HOME;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_AUTOCAL_2:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_PIDS:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_HOME;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_SET_KP:
		switch (id) {
		case BUTTON1_Pin:
			if (kp - 0.5f <= 0)
				kp = 0;
			else
				kp -= 0.5f;
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_SET_KI;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:
			kp += 0.5f;
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_SET_KI:
		switch (id) {
		case BUTTON1_Pin:
			if (ki - 0.05f <= 0)
				ki = 0;
			else
				ki -= 0.05f;
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_SET_KD;
			BUZZER_shortPressBeep();
			break;
		case BUTTON3_Pin:
			ki += 0.05f;
			BUZZER_shortPressBeep();
			break;
		}
		break;
	case MENU_SET_KD:
		switch (id) {
		case BUTTON1_Pin:
			if (kd - 0.5f <= 0)
				kd = 0;
			else
				kd -= 0.5f;
			BUZZER_shortPressBeep();
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PIDS;
			storeSettingsToEeprom(kp, ki, kd);
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:
			kd += 0.5f;
			BUZZER_shortPressBeep();
			break;
		}
		break;

	}

//	char str[50];
//	sprintf(str, "short press detected\n");
//	printString(str);
}

void BUTTONS_longPressAction(uint16_t id) {
	/* RESET BUTTON STATE (because event served)*/
	BUTTONS_resetButtonState(id);

	switch (currentMenu) {
	case MENU_HOME:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_TIMER;
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_TIMER:
		switch (id) {
		case BUTTON1_Pin:
			if (minutesLeft > 0) {
				minutesLeft = 0;
				BUZZER_longPressBeep();
			}
			break;
		case BUTTON2_Pin:
			currentMenu = MENU_HOME;
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_PRESETS:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_EDIT_PRESET1:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_EDIT_PRESET2:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_AUTOCAL_0:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_AUTOCAL_1;
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_AUTOCAL_1:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_AUTOCAL_2:
		switch (id) {
		case BUTTON1_Pin:
			currentMenu = MENU_HOME;
			storeSettingsToEeprom(kp, ki, kd);
			printString("PID value stored");
			BUZZER_longPressBeep();

			break;
		case BUTTON2_Pin:

			break;
		case BUTTON3_Pin:
			currentMenu = MENU_HOME;
			printString("PID value discarded");
			BUZZER_longPressBeep();
			break;
		}
		break;
	case MENU_PIDS:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_SET_KP;
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_SET_KP:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PIDS;
			storeSettingsToEeprom(kp, ki, kd);
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_SET_KI:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PIDS;
			storeSettingsToEeprom(kp, ki, kd);
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	case MENU_SET_KD:
		switch (id) {
		case BUTTON1_Pin:

			break;
		case BUTTON2_Pin:
			currentMenu = MENU_PIDS;
			storeSettingsToEeprom(kp, ki, kd);
			BUZZER_longPressBeep();
			break;
		case BUTTON3_Pin:

			break;
		}
		break;
	}

//	char str[50];
//	sprintf(str, "long press detected\n");
//	printString(str);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {	// button released
	if (HAL_GetTick() - lastFall <= SHORT_PRESS_TIME)
		return;	// debounce

	for (int i = 0; i < 3; i++) {
		if (GPIO_Pin == btn[i].id) {
			btn[i].pressed = 0;
			if (btn[i].pressType == NONE) {
				if (HAL_GetTick() - btn[i].t1 < LONG_PRESS_TIME
						&& HAL_GetTick() - btn[i].t1 > SHORT_PRESS_TIME) {
					btn[i].pressType = SHORT_PRESS;
				}
				if (HAL_GetTick() - btn[i].t1 > LONG_PRESS_TIME) {
					btn[i].pressType = LONG_PRESS;
				}
			}

		}
	}

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {	// button pressed
	if (HAL_GetTick() - lastFall <= SHORT_PRESS_TIME)
		return;	// debounce

	for (int i = 0; i < 3; i++) {
		if (GPIO_Pin == btn[i].id) {
			btn[i].pressType = NONE;// there something that keep the previews value so i reset it to 0 when a new press appears
			btn[i].t1 = HAL_GetTick();
			lastFall = btn[i].t1;// we can not use btn.t1 because we need a global debounce not only for the pressed button
			btn[i].pressed = 1;
			break;
		}
	}
}
