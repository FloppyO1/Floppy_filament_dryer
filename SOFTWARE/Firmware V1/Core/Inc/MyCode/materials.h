/*
 * materials.h
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_MATERIALS_H_
#define INC_MYCODE_MATERIALS_H_

#include "stm32g0xx_hal.h"


enum MATERIAL_ID {
	MATERIAL_ID_PLA,
	MATERIAL_ID_PETG,
	MATERIAL_ID_TPU,
	MATERIAL_ID_ABS,
	MATERIAL_ID_NYLON,
	MATERIAL_ID_PC,
	MATERIAL_ID_PP,
	MATERIAL_ID_ASA,
	MATERIAL_ID_CUS_1,
	MATERIAL_ID_CUS_2
};

typedef struct {
	uint8_t id;
	char name[10];
	uint8_t target_temp;
} Material;

void MATERIAL_initMaterials();
uint8_t MATERIAL_getNextMaterial(uint8_t materialID);
uint8_t MATERIAL_getPrevMaterial(uint8_t materialID);
void MATERIAL_getMaterialName(char *str, uint8_t materialID);
uint8_t MATERIAL_getMaterialTargetTemp(uint8_t materialID);

void MATERIAL_setTargetTemp(uint8_t materialID, uint8_t set);

#endif /* INC_MYCODE_MATERIALS_H_ */
