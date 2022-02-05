#include <stdint.h>
#include <stdlib.h>
#include "UART_Definitions.h"
#include "GPIO_Definitions.h"
#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"
#include "myprint.h"
#include "SAI_Module.h"
#include "I2C_Module.h"
#include "GPIO_Module.h"

void init_interrupts(void);
void _init(void){};

void print_clocks(uint32_t(*fp)(void));
void print_clocks_val(uint32_t val);

void main()
{
	//Send UART message "m4" to signal core is alive
	print_string("m4\n", 3);


	
	//Initialize HAL - Required Call
	HAL_Init();
	
	//Initialize peripherals
	init_GPIO();
	
	//Initialize Interrupts
	init_interrupts();
	
	//Initialize I2C
	init_I2C();
	
	//Initialize SAI/I2S
	//SAI_Init(SAI_INIT_I2S);
	
	//DMA Config
	
	
	
	//CODEC Configure + Start
	
	
	
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



void init_interrupts(void)
{
	HAL_NVIC_EnableIRQ(EXTI14_IRQn);	
}


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
