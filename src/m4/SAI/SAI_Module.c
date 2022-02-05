#include "SAI_Module.h"
#include "myprint.h"


void init_SAI(SAI_Init_Mode init_mode, SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib)
{
	if (init_mode == SAI_INIT_I2S)	//Init I2S operation of SAI Module
	{
		__HAL_RCC_SAI2_CLK_ENABLE();
		SAI_Init_BlockA(hsaia);
		SAI_Init_BlockB(hsaib);
	}
	else	//Init MCLK only for testing
	{
		
	}
	
	
}

void SAI_Init_BlockA(SAI_HandleTypeDef *hsaia) //Transmit Block
{
	//Error Check Variable
	HAL_StatusTypeDef SAI_Check;

	
	//Populate Init struct with I2S properties
	hsaia->Instance					= SAI2_Block_A;
	hsaia->Init.AudioMode			= SAI_MODEMASTER_TX;
	hsaia->Init.Synchro 			= SAI_ASYNCHRONOUS; 
	hsaia->Init.SynchroExt			= SAI_SYNCEXT_OUTBLOCKA_ENABLE;
	hsaia->Init.MckOutput			= SAI_MCK_OUTPUT_ENABLE;
	hsaia->Init.OutputDrive			= SAI_OUTPUTDRIVE_DISABLE; //Possibly change!!!
	hsaia->Init.NoDivider			= SAI_MASTERDIVIDER_ENABLE;
	hsaia->Init.FIFOThreshold		= SAI_FIFOTHRESHOLD_EMPTY; //Interrupt when empty
	hsaia->Init.AudioFrequency		= SAI_AUDIO_FREQUENCY_48K;
	hsaia->Init.Mckdiv				= 0;
	hsaia->Init.MckOverSampling		= SAI_MCK_OVERSAMPLING_ENABLE;
	hsaia->Init.MonoStereoMode		= SAI_MONOMODE;
	hsaia->Init.CompandingMode		= SAI_NOCOMPANDING;
	hsaia->Init.TriState			= SAI_OUTPUT_NOTRELEASED;
	
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLL3_Q;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
		print_string("SAIA CLK FAIL\n", 14);
    }
	
	
	//Initialize SAI with I2S protocol
	SAI_Check = HAL_SAI_InitProtocol(	hsaia, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

	if (SAI_Check == HAL_ERROR)
	{
		print_string("SAIA ERROR\n", 11);
	}
	
}

void SAI_Init_BlockB(SAI_HandleTypeDef *hsaib) //Receive Block
{
	//Error Check Variable
	HAL_StatusTypeDef 	SAI_Check;

	
	//Populate Init struct with I2S properties
	hsaib->Instance 				= SAI2_Block_B;
	hsaib->Init.AudioMode			= SAI_MODESLAVE_RX;
	hsaib->Init.Synchro 			= SAI_SYNCHRONOUS; 
	hsaib->Init.SynchroExt			= SAI_SYNCEXT_DISABLE;
	hsaib->Init.MckOutput			= SAI_MCK_OUTPUT_DISABLE;
	hsaib->Init.OutputDrive			= SAI_OUTPUTDRIVE_DISABLE; //Possibly Change
	hsaib->Init.NoDivider			= SAI_MASTERDIVIDER_ENABLE;
	hsaib->Init.FIFOThreshold		= SAI_FIFOTHRESHOLD_FULL; //Interrupt when full
	hsaib->Init.AudioFrequency		= SAI_AUDIO_FREQUENCY_48K;
	hsaib->Init.Mckdiv				= 0;
	hsaib->Init.MckOverSampling		= SAI_MCK_OVERSAMPLING_ENABLE;
	hsaib->Init.MonoStereoMode		= SAI_MONOMODE;
	hsaib->Init.CompandingMode		= SAI_NOCOMPANDING;
	hsaib->Init.TriState			= SAI_OUTPUT_NOTRELEASED;

	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLL3_Q;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
		print_string("SAIB CLK FAIL\n", 14);
    }


	//Initialize SAI with I2S protocol
	SAI_Check = HAL_SAI_InitProtocol(	hsaib, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

	if (SAI_Check == HAL_ERROR)
	{
		print_string("SAIB ERROR\n", 11);
	}
	
}

