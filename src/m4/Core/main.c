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

uint32_t sine_wave[128] = {	0x400000,0x4323ec,0x4645e9,0x496408,0x4c7c5c,0x4f8cfc,0x529406,0x558f9a,
							0x587de2,0x5b5d0f,0x5e2b5c,0x60e70e,0x638e76,0x661fef,0x6899e5,0x6afad2,
							0x6d413c,0x6f6bbd,0x717900,0x7367c0,0x7536cb,0x76e506,0x787165,0x79daf5,
							0x7b20d7,0x7c4241,0x7d3e82,0x7e14fd,0x7ec52f,0x7f4eaa,0x7fb11a,0x7fec43,
							0x7fffff,0x7fec43,0x7fb11a,0x7f4eaa,0x7ec52f,0x7e14fd,0x7d3e82,0x7c4241,
							0x7b20d7,0x79daf5,0x787165,0x76e506,0x7536cb,0x7367c0,0x717900,0x6f6bbd,
							0x6d413c,0x6afad2,0x6899e5,0x661fef,0x638e76,0x60e70e,0x5e2b5c,0x5b5d0f,
							0x587de2,0x558f9a,0x529406,0x4f8cfc,0x4c7c5c,0x496408,0x4645e9,0x4323ec,
							0x400000,0x3cdc13,0x39ba16,0x369bf7,0x3383a3,0x307303,0x2d6bf9,0x2a7065,
							0x27821d,0x24a2f0,0x21d4a3,0x1f18f1,0x1c7189,0x19e010,0x17661a,0x15052d,
							0x12bec3,0x109442,0xe86ff,0xc983f,0xac934,0x91af9,0x78e9a,0x6250a,
							0x4df28,0x3bdbe,0x2c17d,0x1eb02,0x13ad0,0xb155,0x4ee5,0x13bc,
							0x0,0x13bc,0x4ee5,0xb155,0x13ad0,0x1eb02,0x2c17d,0x3bdbe,
							0x4df28,0x6250a,0x78e9a,0x91af9,0xac934,0xc983f,0xe86ff,0x109442,
							0x12bec3,0x15052d,0x17661a,0x19e010,0x1c7189,0x1f18f1,0x21d4a3,0x24a2f0,
							0x27821d,0x2a7065,0x2d6bf9,0x307303,0x3383a3,0x369bf7,0x39ba16,0x3cdc13
						  };

void *sine_wave_ptr = sine_wave;

void init_interrupts(void);
void _init(void){};

void print_clocks(uint32_t(*fp)(void));
void print_clocks_val(uint32_t val);

HAL_StatusTypeDef GetCODECid(I2C_HandleTypeDef *hi2c1, uint8_t *id);
HAL_StatusTypeDef BootCODEC(I2C_HandleTypeDef *hi2c1);
HAL_StatusTypeDef ConfigureCODEC(I2C_HandleTypeDef *hi2c1);
HAL_StatusTypeDef StartCODEC(I2C_HandleTypeDef *hi2c1);

HAL_StatusTypeDef SendAudio(SAI_HandleTypeDef *hsai);


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
	init_SAI(SAI_INIT_Master, &hsaia, &hsaib);
	
	//DMA Config
	
	
	
	//CODEC Configure + Start
	BootCODEC(&hi2c1);
	ConfigureCODEC(&hi2c1);
	StartCODEC(&hi2c1);
	

	
	//Application
	char str[10] = {0};
	uint8_t id = 0;
	
	if (GetCODECid(&hi2c1, &id) == HAL_OK)
	{
		itoa(id, str, 16);
	
		print_string("CODEC id: ", 10);
		print_string(str, 10);
		print_char('\n');
	}
	
	//SendAudio(&hsaia);
	//hsaia.Instance->CR1 |=  SAI_xCR1_SAIEN;
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);
		SendAudio(&hsaia);
	}
	
}

HAL_StatusTypeDef SendAudio(SAI_HandleTypeDef *hsai)
{
	uint8_t data[4] = {0x40, 0x00, 0x00, 0x00};
	HAL_StatusTypeDef status;
	status = HAL_SAI_Transmit(hsai, sine_wave_ptr, 128, HAL_MAX_DELAY);	
	//status = HAL_SAI_Transmit(hsai, data, 4, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		print_string("Failed audio tx\n", 16); 
	}
	else
	{
		print_string("after SAI tx\n", 13);	
	}
	
	return status;
}

HAL_StatusTypeDef BootCODEC(I2C_HandleTypeDef *hi2c1)
{
	HAL_StatusTypeDef status;
	uint8_t data[2];
	data[0] = 0x02;
	data[1] = 0x01;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
	
	//Send address of PDN register in CODEC
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, data, 2, 500);
	
	if (status != HAL_OK)
	{
		print_string("Failed in PDN tx\n", 17);
	}
	
	return status;
}

HAL_StatusTypeDef ConfigureCODEC(I2C_HandleTypeDef *hi2c1)
{
	HAL_StatusTypeDef status;
	
	//Configuration Values
	uint8_t config_data_codec_master[29] = { 0x82, 0x1F, 0x4E, 0x4C, 0x00, 0xA0, 0x20, 0x60, 0x02, 0x00,
									 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 
									 0x88, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x3F };
	
	uint8_t config_data_codec_slave[29] = { 0x82, 0x1F, 0xAE, 0x0C, 0x00, 0xA0, 0x20, 0x60, 0x02, 0x00,
									 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 
									 0x88, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x3F };
	
	uint8_t status_address 	= 0x20;
	uint8_t status_result	= 0;
	
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, config_data_codec_slave, 29, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		print_string("Failed in CF tx\n", 16);	
		return status;
	}
	
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, &status_address, 1, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		print_string("Failed in ST tx\n", 16);	
		return status;
	}
	
	status = HAL_I2C_Master_Receive(hi2c1, 0x95, &status_result, 1, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		print_string("Failed in ST rx\n", 16);
		return status;
	}
	
	if (status_result != 0)
	{
		print_string("Bad Config\n", 11);	
	}
	
	
	return status;
	
}

HAL_StatusTypeDef StartCODEC(I2C_HandleTypeDef *hi2c1)
{
	HAL_StatusTypeDef status;
	uint8_t data[2];
	data[0] = 0x02;
	data[1] = 0x1E;
	
	//Send address of PDN register in CODEC
	status = HAL_I2C_Master_Transmit(hi2c1, 0x94, data, 2, 500);
	
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
