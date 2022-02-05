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

HAL_StatusTypeDef GetCODECid(I2C_HandleTypeDef *hi2c1, uint8_t *id);
HAL_StatusTypeDef StartCODEC(I2C_HandleTypeDef *hi2c1);

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
	I2C_HandleTypeDef hi2c1;
	init_I2C(&hi2c1);
	
	//Initialize SAI
	SAI_HandleTypeDef hsaia;
	SAI_HandleTypeDef hsaib;
	init_SAI(SAI_INIT_I2S, &hsaia, &hsaib);
	
	//DMA Config
	
	
	
	//CODEC Configure + Start
	StartCODEC(&hi2c1);
	
	//Application
	char str[10] = {0};
	uint8_t id = 0;
	
	if (GetCODECid(&hi2c1, &id) == HAL_OK)
	{
		itoa(id, str, 16);
	
		print_string(str, 10);
	}
	
	while(1)
	{
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);
	}
	
}

HAL_StatusTypeDef StartCODEC(I2C_HandleTypeDef *hi2c1)
{
	HAL_StatusTypeDef status;
	uint8_t data[5];
	data[0] = 0x02;
	data[1] = 0x01;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
	
	//Send address of PDN register in CODEC
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, data, 5, 500);
	
	if (status != HAL_OK)
	{
		print_string("Failed in PDN tx\n", 17);
	}
	
	return status;
}

HAL_StatusTypeDef GetCODECid(I2C_HandleTypeDef *hi2c1, uint8_t *id)
{
	
	HAL_StatusTypeDef status;
	uint8_t MAP_ADDR = 0x01;
	
	//Send address of ID register in CODEC
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, &MAP_ADDR, 1, HAL_MAX_DELAY);
	
	if (status != HAL_OK)
	{
		print_string("Failed in ID tx\n", 16);	
		return status;
	}
	
	status = HAL_I2C_Master_Receive(hi2c1, 0x95, id, 1, HAL_MAX_DELAY);
	
	if (status != HAL_OK)
	{
		print_string("Failed in ID rx\n", 16);
		return status;
	}
	
	return status;
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

void NMI_Handler(void)
{
	print_string("NMI\n", 4);
	while(1);
}

void HardFault_Handler(void)
{
	print_string("HardFault\n", 10);
	while(1);
}

void MemManage_Handler(void)
{
	print_string("MemManage\n", 10);
	while(1);
}

void BusFault_Handler(void)
{
	print_string("BusFault\n", 9);
	while(1);
}

void UsageFault_Handler(void)
{
	print_string("UsageFault\n", 11);
	while(1);
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
