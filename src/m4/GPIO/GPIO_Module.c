#include "GPIO_Module.h"

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

static void I2C_GPIO_Config();
static void SAI_GPIO_Config();
static void Misc_GPIO_Config();
static void Enable_Clocks();

void init_GPIO()
{
	Enable_Clocks();
	
	Misc_GPIO_Config();
	I2C_GPIO_Config();
	SAI_GPIO_Config();
		
}

static void Enable_Clocks()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
//	__HAL_RCC_GPIOZ_CLK_ENABLE();	

}

static void I2C_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//PD12
	GPIO_InitStruct.Pin 		= GPIO_PIN_12;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull		= GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF4_I2C1;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//PF15
	GPIO_InitStruct.Pin			= GPIO_PIN_15;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull		= GPIO_PULLUP;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF4_I2C1;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
}


static void SAI_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	

}

static void Misc_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//DeInit Button
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_14);
	
	//Button Enable
	GPIO_InitStruct.Pin 	= GPIO_PIN_14;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//LED Enable
	GPIO_InitStruct.Pin 	= GPIO_PIN_13;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}












