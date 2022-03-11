#include "Audio_Processor.h"

#include <stdlib.h>

#include "stm32mp1xx_hal.h"

#include "Periph_Init.h"
#include "myprint.h"

static int32_t mix_output_buffer[512] = {0};
static int32_t master_output_double_buffer[1024] = {0};      //ISR Access
static int32_t effects_output_buffer[6][512];
static uint8_t output_active = 0;
static volatile ActiveBuffer active_buffer = BUFFER_1;       //ISR Status
static volatile AudioStatus audio_status = AUDIO_STATUS_BUSY;
static volatile ReceiveStatus receive_status = RECEIVE_READY;

static volatile uint32_t receive_samples = 0;
static volatile uint32_t receive_blocks = 0;
static volatile uint8_t number_of_transfers = 0;

static float master_volume = 1.0f;

static uint8_t queued_index[6] = {25, 25, 25, 25, 25, 25};

static SAI_HandleTypeDef hsaia;

static AudioClip *active_clips[6];

SAI_HandleTypeDef *Get_SAIA_Handle()
{
	return &hsaia;
}

void Audio_Processor_Init()
{
	int32_t dummy_send[4] = {0x55, 0x55, 0x55, 0x55};
	void *record_ptr = dummy_send;
	for (int i = 0; i < 6; i++)
	{
		active_clips[i] = NULL;
	}

	//Required to allow SAI Receive
	//SendAudio(hsaia);								
	HAL_SAI_Transmit(Handle_Get_SAIA(), record_ptr, 4, 1500);
}

void Audio_Processor_Start()
{
	if(HAL_SAI_Transmit_DMA(Handle_Get_SAIA(), (int8_t*)master_output_double_buffer, 1024) == HAL_OK)
	{
		print_string("DMA Succ\n", 9);
	}

	Audio_Processor_Resume_Output();

}

void Audio_Processor_Stop()
{
	HAL_SAI_DMAStop(Handle_Get_SAIA());
	memset(mix_output_buffer, 0, 512 * sizeof(int32_t));
	memset(master_output_double_buffer, 0, 1024 * sizeof(int32_t));
	for (int i = 0; i < 6; i++)
	{
		active_clips[i] = NULL;
	}
	Audio_Processor_Pause_Output();

	Audio_Processor_Start();
}

void Audio_Processor_Run()
{
	if (audio_status == AUDIO_STATUS_READY)
	{
		Audio_Processor_Master_Process();
		Audio_Processor_Effects_Process();
		audio_status = AUDIO_STATUS_BUSY;
		//print_char_nl('t');
	}
}

void Audio_Processor_Master_Process()
{
	//Do audio processing
	for (int i = 0; i < 512; i++)
	{
		mix_output_buffer[i] *= master_volume;

		if (mix_output_buffer[i] > 8388607) mix_output_buffer[i] = 8388607;
		if (mix_output_buffer[i] < -8388608) mix_output_buffer[i] = -8388608;
	}

	if(active_buffer == BUFFER_1)
	{
		memcpy(master_output_double_buffer, mix_output_buffer, 512 * sizeof(int32_t));		//active_buffer == BUFFER_1 after 
	}																						//ISR half transfer complete
	else 
	{
		memcpy(master_output_double_buffer + 512, mix_output_buffer, 512 * sizeof(int32_t));
	}

}

void Audio_Processor_Effects_Process()
{

	int number_of_clips = 0;
	if (output_active)
	{
		for (int i = 0; i < 6; i++)
		{
			if(active_clips[i] != NULL)  //Using Effects
			{	
				if (active_clips[i]->use_effects == 1)
				{
					//Do audio processing by calling the effects callback inside effects member of active_clips[i]
				}
				else
				{
					memcpy(effects_output_buffer[number_of_clips], active_clips[i]->read_ptr, 512 * sizeof(int32_t));

					
					
					for (int sample = 0; sample < 512; sample++)
					{
		
						effects_output_buffer[number_of_clips][sample] *= active_clips[i]->volume;


						//if (effects_output_buffer[number_of_clips][sample] > 0 && effects_output_buffer[number_of_clips][sample] < 300) print_char_nl('n');
					}
					

					active_clips[i]->read_ptr += 512;

					if (active_clips[i]->read_ptr >= active_clips[i]->end)			// 	If clip reaches the end, reset read_ptr 
					{																//	and remove clip from active_clips 
						active_clips[i]->read_ptr = active_clips[i]->start;	

						if (active_clips[i]->is_repeating == 0)
						{
							active_clips[i] = NULL;
							queued_index[i] = 25;
						}
					}
				}

				number_of_clips++;		//Should be at end of if statement
			}
		}

		Audio_Processor_Effects_Mix(number_of_clips);
	}
	else	//Populate buffer with 0 values which will output no sound while SAI module runs
	{
		memset(mix_output_buffer, 0, 512 * sizeof(int32_t));
	}
}

void Audio_Processor_Effects_Mix(uint8_t number_of_clips)
{
	//Mix the audio effects clips together
	if (number_of_clips != 1)
	{
		memset(mix_output_buffer, 0, 512 * sizeof(int32_t));

		for (int i = 0; i < number_of_clips; i++)
		{
			for (int k = 0; k < 512; k++)
			{
				mix_output_buffer[k] += effects_output_buffer[i][k];
			}
		}
	}
	else if (number_of_clips == 1)
	{
		memcpy(mix_output_buffer, effects_output_buffer[0], 512 * sizeof(int32_t));
	}


	for (int i = 0; i < 512; i++)
	{
		if (mix_output_buffer[i] > 8388607) mix_output_buffer[i] = 8388607;
		if (mix_output_buffer[i] < -8388608) mix_output_buffer[i] = -8388608;
		//print_char_nl('h');
		//mix_output_buffer[i] += 8388608;
	}
	
}

void Audio_Processor_Add_Clip(uint8_t clip_index)
{
	uint8_t found = 0;
	if (Audio_Processor_Is_Clip_Queued(clip_index) == 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (active_clips[i] == NULL && found == 0)
			{
				AudioClip* audio_clip = Audio_Get_Clip(clip_index);
				audio_clip->read_ptr = audio_clip->start;
				active_clips[i] = audio_clip;
				found = 1;
				queued_index[i] = clip_index;
			}
		}
	}
}

void Audio_Processor_Remove_Clip(uint8_t clip_index)
{
	for (int i = 0; i < 6; i++)
	{
		if (queued_index[i] == clip_index)
		{
			active_clips[i] = NULL;
			queued_index[i] = 25;
		}
	}
}

uint8_t Audio_Processor_Is_Clip_Queued(uint8_t clip_index)
{
	//AudioClip *audio_clip = Audio_Get_Clip(clip_index);
	for (int i = 0; i < 6; i++)
	{
		if (queued_index[i] == clip_index)
		{
			return 1;
		}
	}
	return 0;
}

void Audio_Processor_Sample(uint8_t *continue_sampling, uint8_t index)
{
	int32_t iterations = 0;
	uint8_t end_reached = 0;
	AudioClip *audio_buffer = Audio_Get_Buffer();
	SAI_HandleTypeDef *hsaib = Handle_Get_SAIB();
	while (*continue_sampling != 0 && end_reached == 0)
	{
		HAL_SAI_Receive(Handle_Get_SAIB(), (void*)(audio_buffer->audio + (512 * iterations)), 512, HAL_MAX_DELAY);
		iterations++;

		if (iterations >= 1875)
		{
			end_reached = 1;
		}
	}
	
	audio_buffer->length_32 = iterations * 512;
	audio_buffer->end = audio_buffer->audio + audio_buffer->length_32;
	audio_buffer->start = audio_buffer->audio;
	audio_buffer->read_ptr = audio_buffer->start;
	audio_buffer->is_repeating = 0;
	audio_buffer->use_effects = 0;
	audio_buffer->play_through = 0;
	audio_buffer->volume = 0.25f;


	for (int i = 0; i < audio_buffer->length_32; i++)
	{
		if (audio_buffer->audio[i] & 0x800000)		//Check if value is negative 24-bit
		{
			audio_buffer->audio[i] |= 0xFF000000;
		}
	}


	Audio_Clip_Copy(index, Audio_Get_Buffer_Index());
}

void Audio_Processor_Sample_Start()
{
	if (receive_status == RECEIVE_READY)
	{
		AudioClip *audio_buffer = Audio_Get_Buffer();

		//Start DMA transfer into buffer
		receive_samples = 0;
		receive_status = RECEIVE_BUSY;
		number_of_transfers = 0;
		receive_blocks = 0;

		memset(audio_buffer->audio, 0, 960000 * sizeof(int32_t));
		HAL_SAI_Receive_DMA(Handle_Get_SAIB(), (void*)audio_buffer->audio, 64000);			//Add parameters (buffer)
		print_string("Receive Yes\n", 12);
	}
	else
	{
		print_string("DMA NO\n", 7);
	}
}

void Audio_Processor_Sample_Stop(uint8_t index)
{
	AudioClip *audio_buffer = Audio_Get_Buffer();
	if (receive_status = RECEIVE_BUSY)
	{
		HAL_SAI_DMAStop(Handle_Get_SAIB());

		if ((number_of_transfers + 1) * 64000 > 960000)
		{
			audio_buffer->length_32 = 960000;
		}
		else
		{
			audio_buffer->length_32 = (number_of_transfers + 1) * 64000;
		}

		receive_status = RECEIVE_READY;
	}
	else
	{
		audio_buffer->length_32 = 960000;
	}

	audio_buffer->end = audio_buffer->audio + audio_buffer->length_32;
	audio_buffer->start = audio_buffer->audio;
	audio_buffer->read_ptr = audio_buffer->start;
	audio_buffer->is_repeating = 0;
	audio_buffer->use_effects = 0;
	audio_buffer->play_through = 0;
	audio_buffer->volume = 1.0f;

	for (int i = 0; i < audio_buffer->length_32; i++)
	{
		if (audio_buffer->audio[i] & 0x800000)		//Check if value is negative 24-bit
		{
			audio_buffer->audio[i] |= 0xFF000000;
		}

		audio_buffer->audio[i] *= 0.25;
	}

	Audio_Clip_Copy(index, Audio_Get_Buffer_Index());
}

void Audio_Processor_Modify_Volume(float value, uint8_t direction)
{
	if (direction == 0)	//Down
	{
		if (master_volume - value < 0)
		{
			master_volume = 0.0f;
		}
		else
		{
			master_volume -= value;
		}
	}
	else	//Up
	{
		master_volume += value;
	}
}

void Audio_Processor_Resample_Single(uint8_t clip_index)
{
	//Process clip effects into buffer

	//Copy back from buffer to clip
}


void Audio_Processor_Pause_Output()
{
	output_active = 0;
}
void Audio_Processor_Resume_Output()
{
	output_active = 1;
}

ReceiveStatus Audio_Processor_Get_Receive_Status()
{
	return receive_status;
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsaia)
{
	//print_char_nl('z');
	//Change active buffer
	active_buffer = BUFFER_1;
	//Change Audio Processor state to ready
	audio_status = AUDIO_STATUS_READY;
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsaia)
{
	//print_char_nl('y');
	//Change active buffer
	active_buffer = BUFFER_2;
	//Change Audio Processor state to ready
	audio_status = AUDIO_STATUS_READY;
}

void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(Handle_Get_DMATX());	
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsaib)
{
	//If number (n) of 64000 sample transfer is less than 15, start another transfer at (n*64000 + buffer->audio)
	number_of_transfers++;

	if (number_of_transfers < 15)
	{
		HAL_SAI_Receive_DMA(Handle_Get_SAIB(), (void*)(Audio_Get_Buffer()->audio + (64000*number_of_transfers)), 64000);
		print_char_nl('+');
	}
	else 
	{
		receive_status = RECEIVE_READY;
		receive_samples = 960000;
		print_char_nl('-');
	}

	
}

void DMA1_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(Handle_Get_DMARX());
}