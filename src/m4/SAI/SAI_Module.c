#include "SAI_Module.h"
#include "myprint.h"


void init_SAI(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib)
{
	
	__HAL_RCC_SAI2_CLK_ENABLE();

	SAI_Init_Master(hsaia, hsaib);
	
}

void SAI_Init_Master(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib)
{
	//Error Check Variable
	HAL_StatusTypeDef SAI_Check_A;
	HAL_StatusTypeDef SAI_Check_B;
	
	//Populate Init struct with I2S properties
	hsaib->Instance 				= SAI2_Block_B;
	hsaib->Init.AudioMode			= SAI_MODESLAVE_RX;
	hsaib->Init.Synchro 			= SAI_SYNCHRONOUS; 
	hsaib->Init.SynchroExt			= SAI_SYNCEXT_DISABLE;
	hsaib->Init.MckOutput			= SAI_MCK_OUTPUT_ENABLE;
	hsaib->Init.OutputDrive			= SAI_OUTPUTDRIVE_DISABLE; 
	hsaib->Init.FIFOThreshold		= SAI_FIFOTHRESHOLD_EMPTY; 
	hsaib->Init.AudioFrequency		= SAI_AUDIO_FREQUENCY_48K;
	hsaib->Init.MonoStereoMode		= SAI_MONOMODE;
	hsaib->Init.CompandingMode		= SAI_NOCOMPANDING;
	hsaib->Init.TriState			= SAI_OUTPUT_NOTRELEASED;

	
	//Populate Init struct with I2S properties
	hsaia->Instance					= SAI2_Block_A;
	hsaia->Init.AudioMode			= SAI_MODESLAVE_TX;				//SAI_MODEMASTER_TX
	hsaia->Init.Synchro 			= SAI_ASYNCHRONOUS; 
	hsaia->Init.SynchroExt			= SAI_SYNCEXT_DISABLE; 
	hsaia->Init.MckOutput			= SAI_MCK_OUTPUT_ENABLE;
	hsaia->Init.OutputDrive			= SAI_OUTPUTDRIVE_DISABLE; 
	hsaia->Init.NoDivider			= SAI_MASTERDIVIDER_ENABLE;
	hsaia->Init.FIFOThreshold		= SAI_FIFOTHRESHOLD_EMPTY; 
	hsaia->Init.AudioFrequency		= SAI_AUDIO_FREQUENCY_48K;
	hsaia->Init.Mckdiv				= 2;
	hsaia->Init.MckOverSampling		= SAI_MCK_OVERSAMPLING_DISABLE;
	hsaia->Init.MonoStereoMode		= SAI_MONOMODE;
	hsaia->Init.CompandingMode		= SAI_NOCOMPANDING;
	hsaia->Init.TriState			= SAI_OUTPUT_NOTRELEASED;
	
	
	//Initialize SAI with I2S protocol
	SAI_Check_B = HAL_SAI_InitProtocol(	hsaib, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

		
	//Initialize SAI with I2S protocol
	SAI_Check_A = HAL_SAI_InitProtocol(	hsaia, 
										SAI_I2S_STANDARD,
										SAI_PROTOCOL_DATASIZE_24BIT, 
										2
									);

	
	
	if (SAI_Check_A == HAL_ERROR)
	{
		print_string("SAIA ERROR\n", 11);
	}
	
	if (SAI_Check_B == HAL_ERROR)
	{
		print_string("SAIB ERROR\n", 11);
	}
}

