#include <stdint.h>
#include <stdlib.h>

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

#include "GPIO_Definitions.h"
#include "myprint.h"
#include "UART_Definitions.h"
#include "Audio_Processor.h"
#include "Periph_Init.h"
#include "Audio_Memory.h"
#include "Audio_Display.h"
#include "Func_Display.h"
#include "Timer_Module.h"
#include "State_Machine.h"
#include "Rotary_Module.h"
#include "IR_Module.h"

void init_interrupts(void);
void _init(void){};

void print_clocks(uint32_t(*fp)(void));
void print_clocks_val(uint32_t val);

int8_t load_audio_in_memory(uint32_t * audio_start, uint32_t audio_length);

HAL_StatusTypeDef GetCODECid(I2C_HandleTypeDef *hi2c1, uint8_t *id);
HAL_StatusTypeDef BootCODEC(I2C_HandleTypeDef *hi2c1);
HAL_StatusTypeDef ConfigureCODEC(I2C_HandleTypeDef *hi2c1);
HAL_StatusTypeDef StartCODEC(I2C_HandleTypeDef *hi2c1);


void main()
{
	//Send UART message "m4" to signal core is alive
	print_string("m4\n", 3);

	
	//Initialize HAL - Required Call
	HAL_Init();
	
	//Initialize peripherals
	Peripherals_Init();
	SAI_HandleTypeDef *hsaia = Handle_Get_SAIA();
	SAI_HandleTypeDef *hsaib = Handle_Get_SAIB();
	I2C_HandleTypeDef *hi2c1 = Handle_Get_I2C1();
	DMA_HandleTypeDef *hdmatx = Handle_Get_DMATX();


	//Initialize Interrupts
	init_interrupts();

	//Initialize Trellis boards
	Audio_Display_Init();
	Func_Display_Init();

	//Initialize Timers
	init_Timers();

	//Initialize IR
	IR_Init();
	
	//CODEC Configure + Start
	BootCODEC(hi2c1);
	ConfigureCODEC(hi2c1);
	StartCODEC(hi2c1);
	
	//Initialize Audio Processor
	Audio_Processor_Init();


	uint8_t continue_recording = 1;
	//Audio_Processor_Sample(&continue_recording, 0);

/*
	Audio_Processor_Sample_Start();

	while(Audio_Processor_Get_Receive_Status() == RECEIVE_BUSY);

	Audio_Processor_Sample_Stop(0);

	print_string("First Done\n", 11);


	HAL_Delay(5000);
	Audio_Processor_Start();
	Audio_Processor_Stop();
	//Audio_Processor_Sample(&continue_recording, 1);

	Audio_Processor_Sample_Start();

	while(Audio_Processor_Get_Receive_Status() == RECEIVE_BUSY);

	Audio_Processor_Sample_Stop(1);

	Audio_Clip_Set_Repeating(0, 0);
	Audio_Clip_Set_UseEffects(0, 0);
	Audio_Clip_Set_Volume(0, 0.5f);
	Audio_Clip_Set_Playthrough(0, 1);
	
	
	//Audio_Clip_Set_Repeating(1, 1);
	//Audio_Clip_Set_UseEffects(1, 0);
	Audio_Clip_Set_Volume(1, 0.5f);

	//Audio_Processor_Add_Clip(0);
	//Audio_Processor_Add_Clip(1);
		
		*/

	Rotary_Register_Item(0, 3, MASTER_VOLUME_PARAM);

	Audio_Processor_Start();

	while(1)
	{
		State_Machine_Run();
	}

	Rotary_Unregister(3);
	
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
	uint8_t config_data_codec_master[29] = { 0x82, 0x1D, 0x4E, 0x4C, 0x20, 0xA0, 0x20, 0x60, 0x02, 0x00,
									 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 
									 0x88, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x3F };
	
	uint8_t config_data_codec_slave[29] = { 0x82, 0x1D, 0xAE, 0x0C, 0x20, 0xA0, 0x20, 0x60, 0x02, 0x00,
									 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF0, 0x00, 0x80, 
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
	data[1] = 0x1C;
	
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
	Rotary_Toggle_Granularity();
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
