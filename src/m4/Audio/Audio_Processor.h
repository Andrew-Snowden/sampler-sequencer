#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include "stm32mp1xx_hal.h"

#include "Audio_Structs.h"
#include "Audio_Memory.h"
#include "defines.h"

void Audio_Processor_Effects_Process();
void Audio_Processor_Effects_Mix(uint8_t number_of_clips);
void Audio_Processor_Master_Process();      
void Audio_Processor_Run();

void Audio_Processor_Init();
void Audio_Processor_Start();          //Should only be called after the audio memory is initialized

void Audio_Processor_Pause_Output();
void Audio_Processor_Resume_Output();

void Audio_Processor_Add_Clip(uint8_t clip_index);
void Audio_Processor_Remove_Clip(uint8_t clip_index);

void Audio_Processor_Sample(uint8_t *continue_sampling, uint8_t index);          //Records audio into buffer
void Audio_Processor_Resample_Single(uint8_t clip_index);       //Resamples clip into itself with effects

uint32_t *Get_Output_Buffer();
SAI_HandleTypeDef *Get_SAIA_Handle();

/* TODO: Array of audio effects to apply in Audio_Processor_Master_Process */

#endif