#include "Audio_Processor.h"

#include "stm32mp1xx_hal.h"

#include "Periph_Init.h"
#include "myprint.h"

static uint32_t mix_output_buffer[512] = {0};
static uint32_t master_output_double_buffer[1024] = {0};      //ISR Access
static uint32_t effects_output_buffer[6][512];
static uint8_t output_active = 0;
static volatile ActiveBuffer active_buffer = BUFFER_1;                      //ISR Status
static volatile AudioStatus audio_status = AUDIO_STATUS_BUSY;

static SAI_HandleTypeDef hsaia;

static AudioClip *active_clips[6];

SAI_HandleTypeDef *Get_SAIA_Handle()
{
	return &hsaia;
}

void Audio_Processor_Init()
{
	for (int i = 0; i < 6; i++)
	{
		active_clips[i] = NULL;
	}
}

void Audio_Processor_Start()
{
	if(HAL_SAI_Transmit_DMA(Handle_Get_SAIA(), (uint8_t*)master_output_double_buffer, 1024) == HAL_OK)
	{
		print_string("DMA Succ\n", 9);
	}

	Audio_Processor_Resume_Output();

	print_char_nl('w');
}

void Audio_Processor_Run()
{
	if (audio_status == AUDIO_STATUS_READY)
	{
		Audio_Processor_Master_Process();
		Audio_Processor_Effects_Process();
		audio_status = AUDIO_STATUS_BUSY;
	}
}

void Audio_Processor_Master_Process()
{
	//Do audio processing
	if(active_buffer == BUFFER_1)
	{
		memcpy(master_output_double_buffer, mix_output_buffer, 512 * sizeof(uint32_t));		//active_buffer == BUFFER_1 after 
	}																						//ISR half transfer complete
	else 
	{
		memcpy(master_output_double_buffer + 512, mix_output_buffer, 512 * sizeof(uint32_t));
	}

}

void Audio_Processor_Effects_Process()
{
	int number_of_clips = 0;
	if (output_active)
	{
		print_char_nl('w');
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
					memcpy(effects_output_buffer[number_of_clips], active_clips[i]->read_ptr, 512 * sizeof(uint32_t));
					print_char_nl('s');

					active_clips[i]->read_ptr += 512;

					if (active_clips[i]->read_ptr >= active_clips[i]->end)			// 	If clip reaches the end, reset read_ptr 
					{																//	and remove clip from active_clips 
						active_clips[i]->read_ptr = active_clips[i]->start;	

						if (active_clips[i]->is_repeating == 0) active_clips[i] = NULL;
					}
				}


				number_of_clips++;		//Should be at end of if statement
			}
		}

		Audio_Processor_Effects_Mix(number_of_clips);
	}
	else	//Populate buffer with 0 values which will output no sound while SAI module runs
	{
		memset(mix_output_buffer, 0, 512 * sizeof(uint32_t));
	}
}

void Audio_Processor_Effects_Mix(uint8_t number_of_clips)
{
	//Mix the audio effects clips together
	if (number_of_clips == 0)
	{
		memset(mix_output_buffer, 0, 512 * sizeof(uint32_t));
	}
	else if (number_of_clips == 1)
	{
		memcpy(mix_output_buffer, effects_output_buffer[0], 512 * sizeof(uint32_t));
	}
	else
	{
		//Do mixing
	}
}

void Audio_Process_Add_Clip(uint8_t clip_index)
{
	uint8_t found = 0;
	for (int i = 0; i < 6; i++)
	{
		if (active_clips[i] == NULL && found == 0)
		{
			AudioClip* audio_clip = Audio_Get_Clip(clip_index);
			audio_clip->read_ptr = audio_clip->start;
			active_clips[i] = audio_clip;
			found = 1;
		}
	}
}

void Audio_Process_Remove_Clip(uint8_t clip_index)
{
	active_clips[clip_index] = NULL;
}

void Audio_Processor_Pause_Output()
{
	output_active = 0;
}
void Audio_Processor_Resume_Output()
{
	output_active = 1;
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsaia)
{
	print_char_nl('z');
	//Change active buffer
	active_buffer = BUFFER_1;
	//Change Audio Processor state to ready
	audio_status = AUDIO_STATUS_READY;
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsaia)
{
	print_char_nl('y');
	//Change active buffer
	active_buffer = BUFFER_2;
	//Change Audio Processor state to ready
	audio_status = AUDIO_STATUS_READY;
}

void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(Handle_Get_DMATX());	
}

