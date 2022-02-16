#ifndef AUDIO_STRUCT_H
#define AUDIO_STRUCT_H

#include "defines.h"

const uint32_t init_buffer[BLOCK_SIZE] = {0};

typedef enum buffer_s
{
	BUFFER_1,
	BUFFER_2
} ActiveBuffer;

typedef enum status_s
{
	AUDIO_STATUS_READY,
	AUDIO_STATUS_BUSY
} AudioStatus;

typedef struct Audio_s
{
	SAI_HandleTypeDef * htransmit;
	
	uint32_t buffer_in_1[BLOCK_SIZE];
	uint32_t buffer_in_2[BLOCK_SIZE];
	uint32_t buffer_out_1[BLOCK_SIZE];
	uint32_t buffer_out_2[BLOCK_SIZE];
	
	volatile ActiveBuffer active_buffer;
	volatile AudioStatus audio_status;
	
	uint8_t block_size;
	
	
	
} Audio;

void init_Audio_struct(Audio * haudio)
{
	active_buffer = BUFFER_1;
	haudio->block_size = BLOCK_SIZE;	
	memcpy((void*)buffer_in_1, (const void*)init_buffer, BLOCK_SIZE * 4);
	memcpy((void*)buffer_in_2, (const void*)init_buffer, BLOCK_SIZE * 4);
	memcpy((void*)buffer_out_1, (const void*)init_buffer, BLOCK_SIZE * 4);
	memcpy((void*)buffer_out_2, (const void*)init_buffer, BLOCK_SIZE * 4);
	
}

#endif


























