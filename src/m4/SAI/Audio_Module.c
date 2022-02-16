#include "Audio_Module.h"

void ProcessAudio()
{
	if (audio_def->audio_status == AUDIO_STATUS_BUSY)
	{
		MasterProcess();
		EffectsProcessAndMix();
	}
}

void MasterProcess()
{
	//Send audio out - Start SAI DMA Transmit
	if(audio_def->active_buffer == BUFFER_1)
	{
		HAL_SAI_Transmit_DMA(audio_def->htransmit, (uint8_t *)audio_def->buffer_out_1, audio_def->block_size);
	}
	else
	{
		HAL_SAI_Transmit_DMA(audio_def->htransmit, (uint8_t *)audio_def->buffer_out_2, audio_def->block_size);
	}
	//Do audio processing
	if(audio_def->active_buffer == BUFFER_1)
	{
		memcpy((void*)audio_def->buffer_out_2, (void*)audio_def->buffer_in_2, audio_def->block_size * 4);
	}
	else 
	{
		memcpy((void*)audio_def->buffer_out_1, (void*)audio_def->buffer_in_1, audio_def->block_size * 4);
	}

}

void EffectsProcessAndMix()
{
	
}
















