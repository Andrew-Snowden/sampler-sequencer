#include <stdint.h>
#include <stdlib.h>
#include "UART_Definitions.h"
#include "GPIO_Definitions.h"
#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"
#include "myprint.h"
#include "SAI_Module.h"

void init_peripherals(void);
void init_interrupts(void);
void init_i2c(void);
void _init(void){};

void print_clocks(uint32_t(*fp)(void))
{
	char str[20] = {0};
	int val;
	
	val = fp();
	itoa(val, str, 10);
	
	print_string(str, 20);
	print_char('\n');
}

void print_clocks_val(uint32_t val)
{
	char str[20] = {0};
	
	itoa(val, str, 10);
	
	print_string(str, 20);
	print_char('\n');
}

void main()
{
	//Send UART message "m4" to signal core is alive
	print_string("m4\n", 3);


	print_string("MPU FREQ: ", 10);
	print_clocks(HAL_RCC_GetMPUSSFreq);
	
	print_string("AXI FREQ: ", 10);
	print_clocks(HAL_RCC_GetAXISSFreq);
	
	print_string("MCU FREQ: ", 10);
	print_clocks(HAL_RCC_GetMCUSSFreq);
	
	print_string("SCC FREQ: ", 10);
	print_clocks(HAL_RCC_GetSystemCoreClockFreq);
	
	print_string("P4Q FREQ: ", 10);
	PLL4_ClocksTypeDef PLL4_Clocks;
	HAL_RCC_GetPLL4ClockFreq(&PLL4_Clocks);
	print_clocks_val(PLL4_Clocks.PLL4_Q_Frequency);
	
	print_string("P3Q FREQ: ", 10);
	PLL3_ClocksTypeDef PLL3_Clocks;
	HAL_RCC_GetPLL3ClockFreq(&PLL3_Clocks);
	print_clocks_val(PLL3_Clocks.PLL3_Q_Frequency);
	
	//Initialize HAL - Required Call
	HAL_Init();
	
	//Initialize peripherals
	init_peripherals();
	
	//Initialize Interrupts
	init_interrupts();
	
	//Initialize I2C
	
	
	//Initialize SAI/I2S
	//SAI_Init(SAI_INIT_I2S);
	
	//DMA Config
	
	
	
	//CODEC Configure + Start
	
	
	
	//Infinite loop to test interrupts
	while(1);
	
}

void init_i2c(void)
{
	
}

void SysTick_Handler(void)
{
	HAL_IncTick();	
}

void EXTI14_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);
}

void init_peripherals(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//DeInit Button
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_14);
	
	//Button Enable
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//LED Enable
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
}

void init_interrupts(void)
{
	HAL_NVIC_EnableIRQ(EXTI14_IRQn);	
}
