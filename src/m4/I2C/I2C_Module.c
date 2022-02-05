#include "I2C_Module.h"

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"
#include "myprint.h"

void init_I2C(I2C_HandleTypeDef* hi2c1)
{	
	hi2c1->Instance 			= I2C1;
	hi2c1->Init.Timing 			= 0x10707DBC;	//Value from CubeMX
	hi2c1->Init.OwnAddress1 	= 0x0;
	hi2c1->Init.AddressingMode	= I2C_ADDRESSINGMODE_7BIT;
	hi2c1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1->Init.OwnAddress2		= 0x0;
	hi2c1->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1->Init.GeneralCallMode	= I2C_GENERALCALL_DISABLE;
	hi2c1->Init.NoStretchMode	= I2C_NOSTRETCH_DISABLE;
	
	__HAL_RCC_I2C1_CLK_ENABLE();
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C12;
	PeriphClkInit.I2c12ClockSelection = RCC_I2C12CLKSOURCE_HSI;
	
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		print_string("Periph Clk Fail\n", 16); 	
	}
	
	if(HAL_I2C_Init(hi2c1) != HAL_OK)
	{
		print_string("bad\n", 4);	
	}
/*	
	if(HAL_I2CEx_ConfigAnalogFilter(hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		print_string("bad2\n", 5);
	}
	
	if (HAL_I2CEx_ConfigDigitalFilter(hi2c1, 0) != HAL_OK)
  	{
    	print_string("bad3\n", 5);
  	}
*/

	
}