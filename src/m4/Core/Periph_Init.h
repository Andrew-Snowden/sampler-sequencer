#ifndef PERIPH_HANDLES_H
#define PERIPH_HANDLES_H

#include "stm32mp1xx_hal.h"
#include "stm32mp1xx.h"

void Peripherals_Init();

SAI_HandleTypeDef *Handle_Get_SAIA();
SAI_HandleTypeDef *Handle_Get_SAIB();

I2C_HandleTypeDef *Handle_Get_I2C1();

DMA_HandleTypeDef *Handle_Get_DMATX();
DMA_HandleTypeDef *Handle_Get_DMARX();

#endif