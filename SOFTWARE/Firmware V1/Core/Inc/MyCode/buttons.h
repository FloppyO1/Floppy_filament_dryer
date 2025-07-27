/*
 * buttons.h
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_BUTTONS_H_
#define INC_MYCODE_BUTTONS_H_

enum PRESS_TYPE{ NONE, SHORT_PRESS, LONG_PRESS};

typedef struct {
	uint16_t id;	// gpio pin vector
	uint32_t t1;
	uint8_t pressed;
	uint8_t pressType;
}BUTTON;

void initButtons();
uint8_t getButtonState(uint16_t id);
uint8_t BUTTONS_tick();

void BUTTONS_longPressAction(uint16_t id);
void BUTTONS_shortPressAction(uint16_t id);

#endif /* INC_MYCODE_BUTTONS_H_ */
