/*
 * materials.c
 *
 *  Created on: Jul 11, 2025
 *      Author: Filippo Castellan
 */

#include <string.h>

#include "MyCode/materials.h"

#define MATERIALS_NUMBER 10
Material materialsLibrary[MATERIALS_NUMBER];

void MATERIAL_initMaterials() {
	/* PLA */
	materialsLibrary[0].id = MATERIAL_ID_PLA;
	sprintf(materialsLibrary[0].name, "PLA");
	materialsLibrary[0].target_temp = 60;

	/* PETG */
	materialsLibrary[1].id = MATERIAL_ID_PETG;
	sprintf(materialsLibrary[1].name, "PETG");
	materialsLibrary[1].target_temp = 70;

	/* TPU */
	materialsLibrary[2].id = MATERIAL_ID_TPU;
	sprintf(materialsLibrary[2].name, "TPU");
	materialsLibrary[2].target_temp = 55;

	/* ABS */
	materialsLibrary[3].id = MATERIAL_ID_ABS;
	sprintf(materialsLibrary[3].name, "ABS");
	materialsLibrary[3].target_temp = 65;

	/* NYLON */
	materialsLibrary[4].id = MATERIAL_ID_NYLON;
	sprintf(materialsLibrary[4].name, "NYLON");
	materialsLibrary[4].target_temp = 70;

	/* PC */
	materialsLibrary[5].id = MATERIAL_ID_PC;
	sprintf(materialsLibrary[5].name, "PC");
	materialsLibrary[5].target_temp = 70;

	/* PP */
	materialsLibrary[6].id = MATERIAL_ID_PP;
	sprintf(materialsLibrary[6].name, "PP");
	materialsLibrary[6].target_temp = 70;

	/* ASA */
	materialsLibrary[7].id = MATERIAL_ID_ASA;
	sprintf(materialsLibrary[7].name, "ASA");
	materialsLibrary[7].target_temp = 60;

	/* CUSTOM 1 */
	materialsLibrary[8].id = MATERIAL_ID_CUS_1;
	sprintf(materialsLibrary[8].name, "CUS.1");
	materialsLibrary[8].target_temp = 50;

	/* CUSTOM 2 */
	materialsLibrary[9].id = MATERIAL_ID_CUS_2;
	sprintf(materialsLibrary[9].name, "CUS.2");
	materialsLibrary[9].target_temp = 50;

}

uint8_t MATERIAL_getNextMaterial(uint8_t materialID) {
	uint8_t currentId = materialID;
	currentId = (currentId + 1) % MATERIALS_NUMBER;
	return currentId;
}

void MATERIAL_setTargetTemp(uint8_t materialID, uint8_t set){
	materialsLibrary[materialID].target_temp = set;
}

uint8_t MATERIAL_getPrevMaterial(uint8_t materialID) {
	uint8_t currentId = materialID;
	currentId = (currentId +  MATERIALS_NUMBER - 1) % MATERIALS_NUMBER;
	return currentId;
}

void MATERIAL_getMaterialName(char *str, uint8_t materialID){
	strcpy(str, materialsLibrary[materialID].name);
}

uint8_t MATERIAL_getMaterialTargetTemp(uint8_t materialID){
	return materialsLibrary[materialID].target_temp;
}
