#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include "Audio_Structs.h"
#include "Audio_Memory.h"
#include "defines.h"


static uint32_t mix_output_buffer[512];
static uint32_t master_output_double_buffer[1024];
static uint32_t effects_output_buffer[6][512];
static uint8_t output_active;

static AudioClip *active_clips[6];

void Audio_Processor_Effects_Process();
void Audio_Processor_Effects_Mix(uint8_t number_of_clips);
void Audio_Processor_Master_Process();      
void Audio_Processor_Run();

void Audio_Processor_Init_And_Start();          //Should only be called after the audio memory is initialized

void Audio_Processor_Pause_Output();
void Audio_Processor_Resume_Output();

void Audio_Process_Add_Clip(uint8_t clip_index);
void Audio_Process_Remove_Clip(uint8_t clip_index);

uint32_t *Get_Output_Buffer();

/* TODO: Array of audio effects to apply in Audio_Processor_Master_Process */

#endif