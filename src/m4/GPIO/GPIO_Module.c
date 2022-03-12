#include "GPIO_Module.h"

#include "stm32mp1xx_hal.h"

static void CODEC_GPIO_Config();
static void I2C_GPIO_Config();
static void SAI_GPIO_Config();
static void Rotary_GPIO_Config();
static void Misc_GPIO_Config();
static void Enable_Clocks();

void init_GPIO()
{
	Enable_Clocks();
	
	Misc_GPIO_Config();
	CODEC_GPIO_Config();
	Rotary_GPIO_Config();
}

static void Enable_Clocks()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
//	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
//	__HAL_RCC_GPIOZ_CLK_ENABLE();	

}

static void CODEC_GPIO_Config()
{
	I2C_GPIO_Config();
	SAI_GPIO_Config();
	
	//Configure CODEC-specific GPIO
	
	//PG9 - Reset (Active Low)
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin			= GPIO_PIN_9;
	GPIO_InitStruct.Mode		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);
}

static void I2C_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//PD12 - SCL
	GPIO_InitStruct.Pin 		= GPIO_PIN_12;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull		= GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate	= GPIO_AF5_I2C1;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//PF15 - SDA
	GPIO_InitStruct.Pin			= GPIO_PIN_15;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull		= GPIO_PULLUP;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate	= GPIO_AF5_I2C1;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);	
}


static void SAI_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//PE0 - MCLKA
	GPIO_InitStruct.Pin			= GPIO_PIN_0;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF10_SAI2;
	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//PI5, PI6, PI7 - SCKA, SDA, FSA
	GPIO_InitStruct.Pin			= GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_6;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF10_SAI2;
	
	
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	//PF11 - SDB
	GPIO_InitStruct.Pin			= GPIO_PIN_11;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF10_SAI2;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

static void Rotary_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//R0UP - PF8
	GPIO_InitStruct.Pin 	= GPIO_PIN_8;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	//R0DOWN - PF6
	GPIO_InitStruct.Pin 	= GPIO_PIN_6;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	//R1UP - PD7
	GPIO_InitStruct.Pin 	= GPIO_PIN_7;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	//R1DOWN - PG15
	GPIO_InitStruct.Pin 	= GPIO_PIN_15;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	//R2UP - PF1
	GPIO_InitStruct.Pin 	= GPIO_PIN_1;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	//R2DOWN - PF0
	GPIO_InitStruct.Pin 	= GPIO_PIN_0;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	//R3UP - PF4
	GPIO_InitStruct.Pin 	= GPIO_PIN_4;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	//R3DOWN - PF5
	GPIO_InitStruct.Pin 	= GPIO_PIN_5;
	GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
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

	//Trellis Int
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//IR Reciever
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//LED Enable
	GPIO_InitStruct.Pin 	= GPIO_PIN_13;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}














