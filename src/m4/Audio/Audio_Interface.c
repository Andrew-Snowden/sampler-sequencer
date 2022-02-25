#include "Audio_Interface.h"


void Audio_Process_Add_Clip(AudioSamples *audio_samples, uint8_t clip_index, uint8_t is_repeating)
{

}

void Audio_Process_Remove_Clip(uint8_t clip_index)
{

}


/*
*   Function: Audio_Parse_From_File
*   Description: Parse file from SD card into buffer to be played and loaded with Audio_Loaded_Into_Memory() if selected
*/
StatusType Audio_Parse_From_File(AudioLoadObject *audio_load_object)
{

}

/*
*	Function: init_Audio_struct
*	Description: Initializes audio struct with default values. Important for proper operation
*	Arguments: Audio * haudio
*/
void Audio_Struct_init(AudioMasterBuffer *haudio)
{
	uint32_t init_buffer[BLOCK_SIZE] = {0};

	haudio->active_buffer = BUFFER_1;
	haudio->block_size = BLOCK_SIZE;	
	memcpy(haudio->buffer_in_1,  init_buffer, BLOCK_SIZE * 4);		//4 bytes per sample
	memcpy(haudio->buffer_in_2,  init_buffer, BLOCK_SIZE * 4);
	memcpy(haudio->buffer_out_1, init_buffer, BLOCK_SIZE * 4);
	memcpy(haudio->buffer_out_2, init_buffer, BLOCK_SIZE * 4);
}