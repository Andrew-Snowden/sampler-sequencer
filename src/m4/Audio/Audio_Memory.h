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
    int32_t    audio[960000];
    int32_t    length_32;                         //Length of clip (number of 32-bit samples)
    int32_t    is_allocated;

    int32_t    *start;                            //Where clip starts when played
    int32_t    *end;                              //Where clip ends when played
    int32_t    *read_ptr;                         //Current read location. Used by effects processor

    uint8_t     is_repeating;                      //Indicates if clip should repeat when end is reached
    uint8_t     use_effects;
    uint8_t     play_through;                       //Indicates that a sample should play to the end (play_through = 1)

    float       volume;
    /* Effects object containing multiple effects that can be allocated */
    /* Effects effects; */


} AudioClip;

//Access - Functions: Copy, Load, Delete, Move
StatusType Audio_Clip_Load(uint8_t index_destination, int32_t *source_address, int32_t number_of_samples);
StatusType Audio_Load_Buffer(int32_t *source, int32_t number_of_samples, int32_t sample_offset);
void Audio_Clip_Copy(uint8_t index_destination, uint8_t index_source);
void Audio_Clip_Delete(uint8_t index);
void Audio_Clip_Move(uint8_t index_destination, uint8_t index_source);

void Audio_Clip_Set_Repeating(uint8_t index, uint8_t is_repeating);
void Audio_Clip_Toggle_Repeating(uint8_t index);
void Audio_Clip_Set_UseEffects(uint8_t index, uint8_t use_effects);
void Audio_Clip_Set_Playthrough(uint8_t index, uint8_t play_through);
void Audio_Clip_Set_Volume(uint8_t index, float volume);

void Audio_Clip_Reset_Start(uint8_t index);
void Audio_Clip_Reset_End(uint8_t index);
void Audio_Clip_Modify_Start(uint8_t index, int32_t number_of_blocks, ClipModifyDirection direction);
void Audio_Clip_Modify_End(uint8_t index, int32_t number_of_blocks, ClipModifyDirection direction);

uint8_t Audio_Is_Slot_Free(uint8_t index);
uint8_t Audio_Get_Buffer_Index();

AudioClip *Audio_Get_Clip(uint8_t index);
AudioClip *Audio_Get_Buffer();


void Audio_Memory_Init();


#endif