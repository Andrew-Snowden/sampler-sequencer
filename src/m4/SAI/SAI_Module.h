#ifndef SAI_MODULE_H
#define SAI_MODULE_H

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

typedef enum 
{
	SAI_INIT_Master,
	SAI_INIT_Slave
} SAI_Init_Mode;

void init_SAI(SAI_Init_Mode mode, SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib);

void SAI_Init_BlockA_Slave();
void SAI_Init_BlockA_Master();

void SAI_Init_BlockB();

void HAL_SAI_MspInitBlockA(SAI_HandleTypeDef *hsai);

void HAL_SAI_MspInitBlockB(SAI_HandleTypeDef *hsai);

#endif