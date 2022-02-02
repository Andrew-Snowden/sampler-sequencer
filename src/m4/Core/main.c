#include <stdint.h>
#include "UART_Definitions.h"
#include "GPIO_Definitions.h"
#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

inline void uart_wait(void);
void init_peripherals(void);
void init_interrupts(void);
void _init(void){};

void main()
{
	//Initialize HAL - Required Call
	HAL_Init();
	
	//Initialize peripherals
	init_peripherals();
	
	//Initialize Interrupts
	init_interrupts();
	
	//Send UART message "m4" to signal core is alive
	uart_wait();
	UART4_TDR = 'm';
	uart_wait();
	UART4_TDR = '4';
	
	//Infinite loop to test interrupts
	while(1);
	
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

void uart_wait(void)
{
	while((UART4_ISR & ISR_TXFT_MASK) == 0);
}