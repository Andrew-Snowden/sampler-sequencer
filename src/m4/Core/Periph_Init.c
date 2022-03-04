#include "Periph_Init.h"

#include "stm32mp1xx_hal.h"

#include "I2C_Module.h"
#include "GPIO_Module.h"
#include "SAI_Module.h"

static SAI_HandleTypeDef hsaia;
static SAI_HandleTypeDef hsaib;
static I2C_HandleTypeDef hi2c1;
static DMA_HandleTypeDef hdmatx;
static DMA_HandleTypeDef hdmarx;

void Peripherals_Init()
{
    init_GPIO();
    init_I2C(&hi2c1);
    init_SAI(&hsaia, &hsaib, &hdmatx, &hdmarx);
}

SAI_HandleTypeDef *Handle_Get_SAIA()
{
    return &hsaia;
}
SAI_HandleTypeDef *Handle_Get_SAIB()
{
    return &hsaib;
}

I2C_HandleTypeDef *Handle_Get_I2C1()
{
    return &hi2c1;
}

DMA_HandleTypeDef *Handle_Get_DMATX()
{
    return &hdmatx;
}

DMA_HandleTypeDef *Handle_Get_DMARX()
{
    return &hdmarx;
}