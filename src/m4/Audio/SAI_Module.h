#ifndef SAI_MODULE_H
#define SAI_MODULE_H

#include <stdint.h>

#include "stm32mp1xx_hal.h"


void init_SAI(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib, DMA_HandleTypeDef *hdmatx, DMA_HandleTypeDef *hdmarx);

void SAI_Init_Master();
void SAI_Init_DMA();

#endif