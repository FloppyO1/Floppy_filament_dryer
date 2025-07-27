/*
 * display.h
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_DISPLAY_H_
#define INC_MYCODE_DISPLAY_H_

#include "stm32g0xx_hal.h"
#include "MyCode/materials.h"

enum MENUS {
	MENU_HOME,
	MENU_TIMER,
	MENU_PRESETS,
	MENU_EDIT_PRESET1,
	MENU_EDIT_PRESET2,
	MENU_AUTOCAL_0,
	MENU_AUTOCAL_1,
	MENU_AUTOCAL_2,
	MENU_PIDS,
	MENU_SET_KP,
	MENU_SET_KI,
	MENU_SET_KD,
};

void DISPLAY_drawMenu(uint8_t currentMenu, uint8_t curretMaterialID, float temperature, uint16_t minuteLeft, uint8_t duty);
uint8_t DISPLAY_intermittentState();

void DISPLAY_PrintFloatSmall (int x, int y, float n);

void DISPLAY_PrintStrSmall(uint8_t *str, uint8_t x, uint8_t y);
void DISPLAY_PrintStrMedium(uint8_t *str, uint8_t x, uint8_t y);
void DISPLAY_PrintStrLarge(uint8_t *str, uint8_t x, uint8_t y);
void DISPLAY_PrintStrHuge(uint8_t *str, uint8_t x, uint8_t y);

uint8_t calculateMinute(uint16_t t);
uint8_t calculateHours(uint16_t t);

#endif /* INC_MYCODE_DISPLAY_H_ */
