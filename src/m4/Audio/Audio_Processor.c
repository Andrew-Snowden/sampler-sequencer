#include "Audio_Processor.h"

void Audio_Processor_Run()
{
	if (audio_status == AUDIO_STATUS_READY)
	{
		Audio_Processor_Master_Process();
		Audio_Processor_Effects_Process();
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
		for (int i = 0; i < 6; i++)
		{
			if(active_clips[i] != NULL && active_clips[i]->use_effects == 1)  //Using Effects
			{
				//Do audio processing by calling the effects callback inside effects member of active_clips[i]
				
				number_of_clips++;		//Should be at end of if statement
			}
			else if (active_clips[i] != NULL)	//Not using effects
			{
				memcpy(effects_output_buffer[number_of_clips], active_clips[i]->read_ptr, 512 * sizeof(uint32_t));
				active_clips[i]->read_ptr += 512;

				if (active_clips[i]->read_ptr >= active_clips[i]->end)			// 	If clip reaches the end, reset read_ptr 
				{																//	and remove clip from active_clips 
					active_clips[i]->read_ptr = active_clips[i]->start;	

					if (active_clips[i]->is_repeating == 0) active_clips[i] = NULL;
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

	if (number_of_clips == 1)
	{
		memcpy(mix_output_buffer, effects_output_buffer[0], 512 * sizeof(uint32_t));
	}
	else
	{
		//Do mixing
	}
}














