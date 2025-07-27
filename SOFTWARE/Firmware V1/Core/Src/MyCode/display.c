/*
 * display.c
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#include <stdio.h>
#include "MyCode/display.h"
#include "Libraries/Display/ssd1306.h"
#include "MyCode/materials.h"
#include "MyCode/config.h"
#include "MyCode/pid.h"

extern float kp;
extern float ki;
extern float kd;

void DISPLAY_drawMenu(uint8_t currentMenu, uint8_t curretMaterialID, float temperature, uint16_t minuteLeft, uint8_t duty) {
	ssd1306_Fill(Black);
	uint8_t str[50];
	switch (currentMenu) {
		case MENU_HOME:
			//DISPLAY_PrintStrMedium("MENU_HOME", 0, 0);
			// draw temp
			sprintf(str, "%0.1f C", temperature);
			DISPLAY_PrintStrLarge(str, 5, 8);

			// draw material or timer
			if (DISPLAY_intermittentState()) {
				// draw material set
				MATERIAL_getMaterialName(str, curretMaterialID);
				DISPLAY_PrintStrMedium(str, 90, 5);
				sprintf(str, "%d C", MATERIAL_getMaterialTargetTemp(curretMaterialID));
				DISPLAY_PrintStrMedium(str, 90, 20);
				ssd1306_DrawCircle(108, 20, 1, White);
			} else {
				// draw eta time
				DISPLAY_PrintStrMedium("ETA:", 83, 5);
				sprintf(str, "%dh%dm", calculateHours(minuteLeft), calculateMinute(minuteLeft));
				DISPLAY_PrintStrMedium(str, 83, 20);
			}
			ssd1306_DrawCircle(56, 9, 2, White);
			ssd1306_Line(79, 0, 79, 32, White);
			/* POWER INDICATOR LINE */
			ssd1306_DrawRectangle(0, 29, duty / 3.5, 31, White);
			break;
		case MENU_TIMER:
			//DISPLAY_PrintStrMedium("MENU_TIMER", 0, 0);
			DISPLAY_PrintStrLarge("TIMER SET:", 0, 0);
			sprintf(str, "< %02dh%02dm >", calculateHours(minuteLeft), calculateMinute(minuteLeft));
			DISPLAY_PrintStrMedium(str, 29, 22);
			ssd1306_Line(35, 23, 35, 29, White);
			ssd1306_Line(92, 23, 92, 29, White);
			break;
		case MENU_PRESETS:
			//DISPLAY_PrintStrMedium("MENU_PRESETS", 0, 0);
			SSD1306_VERTEX left_arrow[] = { { 16, 6 }, { 0, 16 }, { 16, 26 }, { 16, 6 } };
			ssd1306_Polyline(left_arrow, sizeof(left_arrow) / sizeof(left_arrow[0]), White);
			SSD1306_VERTEX right_arrow[] = { { 111, 6 }, { 127, 16 }, { 111, 26 }, { 111, 6 } };
			ssd1306_Polyline(right_arrow, sizeof(right_arrow) / sizeof(right_arrow[0]), White);
			ssd1306_Line(20, 5, 107, 5, White);
			ssd1306_Line(20, 27, 107, 27, White);
			// draw material set
			MATERIAL_getMaterialName(str, curretMaterialID);
			DISPLAY_PrintStrMedium(str, 27, 12);
			sprintf(str, "%d C", MATERIAL_getMaterialTargetTemp(curretMaterialID));
			DISPLAY_PrintStrMedium(str, 72, 12);

			break;
		case MENU_EDIT_PRESET1:
			//DISPLAY_PrintStrMedium("MENU_EDIT_PRESET1", 0, 0);
			sprintf(str, "CUSTOM 1: < %d C >", MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_1));
			DISPLAY_PrintStrMedium(str, 0, 13);
			ssd1306_Line(76, 14, 76, 20, White);
			ssd1306_Line(119, 14, 119, 20, White);
			break;
		case MENU_EDIT_PRESET2:
//			DISPLAY_PrintStrMedium("MENU_EDIT_PRESET2", 0, 0);
			sprintf(str, "CUSTOM 2: < %d C >", MATERIAL_getMaterialTargetTemp(MATERIAL_ID_CUS_2));
			DISPLAY_PrintStrMedium(str, 0, 13);
			ssd1306_Line(76, 14, 76, 20, White);
			ssd1306_Line(119, 14, 119, 20, White);
			break;
		case MENU_AUTOCAL_0:
			//DISPLAY_PrintStrMedium("MENU_AUTOCAL_0", 0, 0);
			DISPLAY_PrintStrLarge("AUTOCAL", 25, 0);
			DISPLAY_PrintStrMedium("HOLD OK TO START", 8, 22);
			break;
		case MENU_AUTOCAL_1:
			//DISPLAY_PrintStrMedium("MENU_AUTOCAL_1", 0, 0);
			DISPLAY_PrintStrMedium("PID AUTOCAL", 0, 5);
			DISPLAY_PrintStrMedium("IS STARTING", 0, 20);
			ssd1306_UpdateScreen();
			HAL_Delay(2000);
			DISPLAY_PrintStrMedium("IT CAN TAKE", 0, 5);
			DISPLAY_PrintStrMedium("SEVERAL MINUTES", 0, 20);
			ssd1306_UpdateScreen();
			HAL_Delay(2000);
			PIDautoCal(PID_AUTOCAL_TEMPERATURE, PID_AUTOTUNE_HYSTERESIS, PID_AUTOCAL_CYCLES, &kp, &ki, &kd);
			break;
		case MENU_AUTOCAL_2:
			//DISPLAY_PrintStrMedium("MENU_AUTOCAL_2", 0, 0);
			if (DISPLAY_intermittentState()) {
				DISPLAY_PrintStrMedium("CALIB. FINISCHED!", 0, 5);
				DISPLAY_PrintStrMedium(">PRESS OK BUTTON<", 0, 20);
				DISPLAY_PrintStrMedium("<SAVE      DISC.>", 0, 20);
			} else {
				DISPLAY_PrintStrMedium("NEW PID VALUE ARE:", 0, 0);
				sprintf(str, "KP=%0.3f KI=%0.3f", kp, ki);
				DISPLAY_PrintStrSmall(str, 0, 12);
				sprintf(str, "KD=%0.3f", kd);
				DISPLAY_PrintStrSmall(str, 0, 22);
			}
			break;
		case MENU_PIDS:
			//DISPLAY_PrintStrMedium("MENU_AUTOCAL_2", 0, 0);
			DISPLAY_PrintStrMedium("PID VALUES:", 0, 0);
			sprintf(str, "KP=%0.3f KI=%0.3f", kp, ki);
			DISPLAY_PrintStrSmall(str, 0, 12);
			sprintf(str, "KD=%0.3f  L.OK EDIT", kd);
			DISPLAY_PrintStrSmall(str, 0, 22);
			break;
		case MENU_SET_KP:
			DISPLAY_PrintStrLarge("SET Kp:", 0, 0);
			sprintf(str, "< %05.2f >", kp);
			DISPLAY_PrintStrMedium(str, 29, 22);
			ssd1306_Line(35, 23, 35, 29, White);
			ssd1306_Line(85, 23, 85, 29, White);
			break;
		case MENU_SET_KI:
			DISPLAY_PrintStrLarge("SET Ki:", 0, 0);
			sprintf(str, "< %05.2f >", ki);
			DISPLAY_PrintStrMedium(str, 29, 22);
			ssd1306_Line(35, 23, 35, 29, White);
			ssd1306_Line(85, 23, 85, 29, White);
			break;
		case MENU_SET_KD:
			DISPLAY_PrintStrLarge("SET Kd:", 0, 0);
			sprintf(str, "< %05.2f >", kd);
			DISPLAY_PrintStrMedium(str, 29, 22);
			ssd1306_Line(35, 23, 35, 29, White);
			ssd1306_Line(85, 23, 85, 29, White);
			break;
	}
}

uint8_t calculateMinute(uint16_t t) {
	return t % 60;
}

uint8_t calculateHours(uint16_t t) {
	return t / 60;
}

uint8_t DISPLAY_intermittentState() {
	uint8_t state = 0;
	uint32_t realTime = HAL_GetTick();
	uint32_t refTime = realTime;
	refTime /= 10000;
	refTime *= 10000;	// now we have the ref point ex: 127522 -> 127000
	if (realTime >= refTime - (STATUS_CHANGE_TIME) && realTime <= refTime + (STATUS_CHANGE_TIME)) {
		state = 1;
	}
	return !state;
}

void DISPLAY_PrintFloatSmall(int x, int y, float n) {
	char str[30];
	sprintf(str, "%0.1f", n);
	DISPLAY_PrintStrHuge(str, x, y);
}

/**
 *  PrintString - Print on screen a String medium dimension (7*10 font)
 *
 *  This function print on screen (SSD1306) an String on the chosen
 *  position by the programmer. It use 7*10 font (medium dimension).
 *
 *  @param str		string to print
 *	@param x		x position on the screen
 *	@param y		y position on the screen
 *
 *	void
 *
 *	Note:
 *	The origin of text are top left.
 *	Example: 	uint8_t string[50];
 *				sprintf(string, "T:%0.1fC ", temperature);
 PrintStrMedium(string, 2, 3);
 */

void DISPLAY_PrintStrSmall(uint8_t *str, uint8_t x, uint8_t y) {
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_6x8, 1);
}

void DISPLAY_PrintStrMedium(uint8_t *str, uint8_t x, uint8_t y) {
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_7x10, 1);
}

void DISPLAY_PrintStrLarge(uint8_t *str, uint8_t x, uint8_t y) {
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_11x18, 1);
}

void DISPLAY_PrintStrHuge(uint8_t *str, uint8_t x, uint8_t y) {
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_16x26, 1);
}

