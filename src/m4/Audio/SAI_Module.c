#include "SAI_Module.h"
#include "myprint.h"


void init_SAI(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib, DMA_HandleTypeDef *hdmatx, DMA_HandleTypeDef *hdmarx)
{
	
	__HAL_RCC_SAI2_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMAMUX_CLK_ENABLE();

	SAI_Init_Master(hsaia, hsaib);
	SAI_Init_DMA(hsaia, hsaib, hdmatx, hdmarx);
}

void SAI_Init_DMA(SAI_HandleTypeDef *hsaia, SAI_HandleTypeDef *hsaib, DMA_HandleTypeDef *hdma_tx, DMA_HandleTypeDef *hdma_rx)
{
	hdma_rx->Instance					= DMA1_Stream1;
	hdma_rx->Init.Request				= DMA_REQUEST_SAI2_B;
	hdma_rx->Init.Direction				= DMA_PERIPH_TO_MEMORY;
	hdma_rx->Init.PeriphInc				= DMA_PINC_DISABLE;
	hdma_rx->Init.MemInc				= DMA_MINC_ENABLE;
	hdma_rx->Init.PeriphDataAlignment	= DMA_PDATAALIGN_WORD;
	hdma_rx->Init.MemDataAlignment		= DMA_MDATAALIGN_WORD;
	hdma_rx->Init.Mode					= DMA_NORMAL;
	hdma_rx->Init.Priority				= DMA_PRIORITY_VERY_HIGH;
	hdma_rx->Init.FIFOMode				= DMA_FIFOMODE_DISABLE;
	hdma_rx->Init.MemBurst				= DMA_MBURST_SINGLE;
	hdma_rx->Init.PeriphBurst			= DMA_PBURST_SINGLE;

	__HAL_LINKDMA(hsaib, hdmarx, *hdma_rx);

	HAL_DMA_Init(hdma_rx);

	hdma_tx->Instance 					= DMA1_Stream0;
	hdma_tx->Init.Request 				= DMA_REQUEST_SAI2_A;
	hdma_tx->Init.Direction 			= DMA_MEMORY_TO_PERIPH;
	hdma_tx->Init.PeriphInc 			= DMA_PINC_DISABLE;
	hdma_tx->Init.MemInc 				= DMA_MINC_ENABLE;
	hdma_tx->Init.PeriphDataAlignment	= DMA_PDATAALIGN_WORD;
	hdma_tx->Init.MemDataAlignment 		= DMA_MDATAALIGN_WORD;
	hdma_tx->Init.Mode 					= DMA_CIRCULAR;
	hdma_tx->Init.Priority 				= DMA_PRIORITY_VERY_HIGH;
	hdma_tx->Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
	hdma_tx->Init.MemBurst 				= DMA_MBURST_SINGLE;
	hdma_tx->Init.PeriphBurst 			= DMA_PBURST_SINGLE;

	__HAL_LINKDMA(hsaia, hdmatx, *hdma_tx);
	//hsaia->hdmatx = hdma_tx;
	//hdma_tx->Parent = hsaia;

	HAL_DMA_Init(hdma_tx);

	

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

	/* Peripheral interrupt init */
	HAL_NVIC_SetPriority(SAI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SAI2_IRQn);

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
	hsaib->Init.OutputDrive			= SAI_OUTPUTDRIVE_DISABLE; 
	hsaib->Init.FIFOThreshold		= SAI_FIFOTHRESHOLD_FULL; 
	hsaib->Init.AudioFrequency		= SAI_AUDIO_FREQUENCY_48K;
	hsaib->Init.MonoStereoMode		= SAI_MONOMODE;
	hsaib->Init.CompandingMode		= SAI_NOCOMPANDING;
	hsaib->Init.TriState			= SAI_OUTPUT_NOTRELEASED;

	
	//Populate Init struct with I2S properties
	hsaia->Instance					= SAI2_Block_A;
	hsaia->Init.AudioMode			= SAI_MODEMASTER_TX;				//SAI_MODEMASTER_TX
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

