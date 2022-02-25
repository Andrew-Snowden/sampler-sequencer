#ifndef AUDIO_INTERFACE_H
#define AUDIO_INTERFACE_H

#include <stdint.h>

#include "defines.h"
#include "Audio_Structs.h"

typedef struct AudioQueued
{
    uint32_t *audio_start_ptr[6];
    uint32_t number_of_samples[6];
    uint8_t clip_index[6];
    uint8_t repeat[6];

} AudioQueued;

static AudioQueued audio_to_process;


StatusType Audio_Parse_From_File(AudioLoadObject *audio_load_object);

void Audio_Struct_init(AudioMasterBuffer *haudio);

void Audio_Process_Add_Clip(AudioSamples *audio_samples, uint8_t clip_index, uint8_t is_repeating);

void Audio_Process_Remove_Clip(uint8_t clip_index);

#endif