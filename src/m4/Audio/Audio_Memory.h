#ifndef AUDIO_MEMORY_H
#define AUDIO_MEMORY_H

#include <stdint.h>

#include "defines.h"

typedef enum ClipModifyDirection
{
    FORWARD,
    BACKWARD
} ClipModifyDirection;

typedef struct AudioClip
{
    uint32_t    audio[960000];
    uint32_t    length_32;                         //Length of clip (number of 32-bit samples)
    uint32_t    is_allocated;

    uint32_t    *start;                            //Where clip starts when played
    uint32_t    *end;                              //Where clip ends when played
    uint32_t    *read_ptr                          //Current read location. Used by effects processor

    uint8_t     is_repeating;                      //Indicates if clip should repeat when end is reached
    uint8_t     use_effects;
    /* Effects object containing multiple effects that can be allocated */
    /* Effects effects; */


} AudioClip;

static AudioClip audio_clips[17];

//Access - Functions: Copy, Load, Delete, Move
StatusType Audio_Clip_Load(uint8_t index_destination, uint32_t *source_address, uint32_t number_of_samples);
void Audio_Clip_Copy(uint8_t index_destination, uint8_t index_source);
void Audio_Clip_Delete(uint8_t index);
void Audio_Clip_Move(uint8_t index_destination, uint8_t index_source);

void Audio_Clip_Set_Repeating(uint8_t index, uint8_t is_repeating);
void Audio_Clip_Set_UseEffects(uint8_t index, uint8_t use_effects);

void Audio_Clip_Reset_Start(uint8_t index);
void Audio_Clip_Reset_End(uint8_t index);
void Audio_Clip_Modify_Start(uint8_t index, uint32_t number_of_blocks, ClipModifyDirection direction);
void Audio_Clip_Modify_End(uint8_t index, uint32_t number_of_blocks, ClipModifyDirection direction);

uint8_t Audio_Is_Slot_Free(uint8_t index);

AudioClip *Get_Audio_Clip(uint8_t index);

void Audio_Memory_Init();


#endif