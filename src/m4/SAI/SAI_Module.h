#ifndef SAI_MODULE_H
#define SAI_MODULE_H

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

typedef enum 
{
	SAI_INIT_MCLK_ONLY,
	SAI_INIT_I2S
} SAI_Init_Mode;

void init_SAI(SAI_Init_Mode init_mode, SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib);

void SAI_Init_BlockA();

void SAI_Init_BlockB();

void HAL_SAI_MspInitBlockA(SAI_HandleTypeDef *hsai);

void HAL_SAI_MspInitBlockB(SAI_HandleTypeDef *hsai);

#endif