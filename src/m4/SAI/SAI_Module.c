#include "SAI_Module.h"
#include "myprint.h"

static void init_peripherals();

void init_SAI(SAI_Init_Mode init_mode)
{
	if (init_mode == SAI_INIT_I2S)	//Init I2S operation of SAI Module
	{
		init_peripherals();	//Enable clock and configure GPIO
		SAI_Init_BlockA();
		SAI_Init_BlockB();
	}
	else	//Init MCLK only for testing
	{
		
	}
	
	
}

void SAI_Init_BlockA() //Transmit Block
{
	//Error Check Variable
	HAL_StatusTypeDef SAI_Check;

	//Declare Handle and Init structs
	SAI_HandleTypeDef 	hsai;
	SAI_InitTypeDef 	SAI_InitStruct;

	//Register MspInit callback for Block A
	//HAL_SAI_RegisterCallback(&hsai, HAL_SAI_MSPINIT_CB_ID, &HAL_SAI_MspInitBlockA);
	
	//Populate Init struct with I2S properties
	SAI_InitStruct.AudioMode		= SAI_MODEMASTER_TX;
	SAI_InitStruct.Synchro 			= SAI_ASYNCHRONOUS; 
	SAI_InitStruct.SynchroExt		= SAI_SYNCEXT_OUTBLOCKA_ENABLE;
	SAI_InitStruct.MckOutput		= SAI_MCK_OUTPUT_ENABLE;
	SAI_InitStruct.OutputDrive		= SAI_OUTPUTDRIVE_DISABLE; //Possibly change!!!
	SAI_InitStruct.NoDivider		= SAI_MASTERDIVIDER_ENABLE;
	SAI_InitStruct.FIFOThreshold	= SAI_FIFOTHRESHOLD_EMPTY; //Interrupt when empty
	SAI_InitStruct.AudioFrequency	= SAI_AUDIO_FREQUENCY_48K;
	SAI_InitStruct.Mckdiv			= 0;
	SAI_InitStruct.MckOverSampling	= SAI_MCK_OVERSAMPLING_ENABLE;
	SAI_InitStruct.MonoStereoMode	= SAI_MONOMODE;
	SAI_InitStruct.CompandingMode	= SAI_NOCOMPANDING;
	SAI_InitStruct.TriState			= SAI_OUTPUT_RELEASED;


	//Insert Init struct into Handle struct
	hsai.Init = SAI_InitStruct;
	hsai.Instance = SAI2_Block_A;

	//Initialize SAI with I2S protocol
	SAI_Check = HAL_SAI_InitProtocol(	&hsai, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

	if (SAI_Check == HAL_ERROR)
	{
		print_string("SAI ERROR", 9);
	}
	
	
}

void SAI_Init_BlockB() //Receive Block
{
	//Error Check Variable
	HAL_StatusTypeDef 	SAI_Check;

	//Declare Handle and Init structs
	SAI_HandleTypeDef 	hsai;
	SAI_InitTypeDef 	SAI_InitStruct;

	//Register MspInit callback for Block B
	//HAL_SAI_RegisterCallback(&hsai, HAL_SAI_MSPINIT_CB_ID, &HAL_SAI_MspInitBlockB);
	
	//Populate Init struct with I2S properties
	SAI_InitStruct.AudioMode		= SAI_MODESLAVE_RX;
	SAI_InitStruct.Synchro 			= SAI_SYNCHRONOUS; 
	SAI_InitStruct.SynchroExt		= SAI_SYNCEXT_DISABLE;
	SAI_InitStruct.MckOutput		= SAI_MCK_OUTPUT_DISABLE;
	SAI_InitStruct.OutputDrive		= SAI_OUTPUTDRIVE_DISABLE; //Possibly Change
	SAI_InitStruct.NoDivider		= SAI_MASTERDIVIDER_ENABLE;
	SAI_InitStruct.FIFOThreshold	= SAI_FIFOTHRESHOLD_FULL; //Interrupt when full
	SAI_InitStruct.AudioFrequency	= SAI_AUDIO_FREQUENCY_48K;
	SAI_InitStruct.Mckdiv			= 0;
	SAI_InitStruct.MckOverSampling	= SAI_MCK_OVERSAMPLING_ENABLE;
	SAI_InitStruct.MonoStereoMode	= SAI_MONOMODE;
	SAI_InitStruct.CompandingMode	= SAI_NOCOMPANDING;
	SAI_InitStruct.TriState			= SAI_OUTPUT_RELEASED;


	//Insert Init struct into Handle struct
	hsai.Init = SAI_InitStruct;
	hsai.Instance = SAI2_Block_B;

	//Initialize SAI with I2S protocol
	SAI_Check = HAL_SAI_InitProtocol(	&hsai, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

	if (SAI_Check == HAL_ERROR)
	{
		print_string("SAI ERROR", 9);
	}
}

static void init_peripherals()
{
	//Enable SAI2 Clock
	__HAL_RCC_SAI2_CLK_ENABLE();
	
	//Enable GPIOx clock
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	//Configure pins
	
	
	//Possibly DMA and IRQ initialization
	
	
}


/*void HAL_SAI_MspInitBlockA(SAI_HandleTypeDef *hsai)
{
	//Enable pclk2 in PCKE register
	
	//Select pll3_q_ck in PKCS register (24.5739 MHz)
	
}

void HAL_SAI_MspInitBlockB(SAI_HandleTypeDef *hsai)
{
	
	
	
}*/