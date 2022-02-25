#ifndef SAI_MODULE_H
#define SAI_MODULE_H

#include <stdint.h>

#include "stm32mp1xx_hal.h"


void init_SAI(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib, DMA_HandleTypeDef *hdmatx);

void SAI_Init_Master();


#endif