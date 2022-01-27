#include <stdint.h>
#include "UART_Definitions.h"
#include "GPIO_Definitions.h"
#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

inline void uart_wait(void);
void init_peripherals(void);
void _init(void){};

void main()
{
	UART4_TDR = 's';
	uart_wait();
	HAL_Init();
	
	init_peripherals();
	
	uart_wait();
	UART4_TDR = 'm';
	uart_wait();
	UART4_TDR = '4';
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);
	
	while(1);
}

void init_peripherals(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
}


void uart_wait(void)
{
	while((UART4_ISR & ISR_TXFT_MASK) == 0);
}