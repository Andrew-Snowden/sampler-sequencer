#include "Audio_Memory.h"

#include <stdint.h>
#include <string.h>

#include "defines.h"

void Audio_Clip_Copy(uint8_t index_destination, uint8_t index_source)
{
    Audio_Clip_Load(index_destination, audio_clips[index_source].audio, audio_clips[index_source].length_32);
}

StatusType Audio_Clip_Load(uint8_t index_destination, uint32_t *source_address, uint32_t number_of_samples)
{
    //Load
    if (Audio_Is_Slot_Free(index_destination))
    {
        Audio_Clip_Delete(index_destination);
        memcpy(audio_clips[index_destination].audio, source_address, number_of_samples * sizeof(uint32_t));

        //Set allocate bit
        audio_clips[index_destination].is_allocated = 1;
    }
    else
    {
        return Status_Fail;
    }
}

void Audio_Clip_Delete(uint8_t index)
{
    memset(audio_clips[index].audio, 0, 960000 * sizeof(uint32_t));

    audio_clips[index].is_allocated = 0;
}

void Audio_Clip_Move(uint8_t index_destination, uint8_t index_source)
{
    Audio_Clip_Copy(index_destination, index_source);
    Audio_Clip_Delete(index_source);
}

uint8_t Audio_Is_Slot_Free(uint8_t index)
{
    if (audio_clips[index].is_allocated == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

AudioClip *Get_Audio_Clip(uint8_t index)
{
    return &audio_clips[index];
}

void Audio_Clip_Set_Repeating(uint8_t index, uint8_t is_repeating)
{
    audio_clips[index].is_repeating = is_repeating;
}

void Audio_Clip_Set_UseEffects(uint8_t index, uint8_t use_effects)
{
    audio_clips[index].use_effects = use_effects;
}

void Audio_Clip_Reset_Start(uint8_t index)
{
    audio_clips[index].start = audio_clips[index].audio;
}

void Audio_Clip_Reset_End(uint8_t index)
{
    audio_clips[index].end = audio_clips[index].audio + audio_clips[index].length_32;
}

void Audio_Clip_Modify_Start(uint8_t index, uint32_t number_of_blocks, ClipModifyDirection direction)
{
    if (direction == FORWARD)
    {
        if ((audio_clips[index].end - 512) >= (audio_clips[index].start + (number_of_blocks * 512)))
        {
            audio_clips[index].start += number_of_blocks * 512;
        }
        else
        {
            audio_clips[index].start = audio_clips[index].end - 512;
        }
    }
    else
    {
        if (audio_clips[index].audio > audio_clips[index].start - (number_of_blocks * 512))
        {
            audio_clips[index].start = audio_clips[index].audio;
        }
        else
        {
            audio_clips[index].start -= number_of_blocks * 512;
        }
    }

}

void Audio_Clip_Modify_End(uint8_t index, uint32_t number_of_blocks, ClipModifyDirection direction)
{
    if (direction == FORWARD)
    {
        if ((audio_clips[index].end + (number_of_blocks * 512)) > (audio_clips[index].audio + audio_clips[index].length_32))
        {
            audio_clips[index].end = audio_clips[index].audio + audio_clips[index].length_32;
        }
        else 
        {
            audio_clips[index].end += number_of_blocks * 512;
        }
    }
    else
    {
        if (audio_clips[index].start >= audio_clips[index].end - (number_of_blocks * 512))
        {
            audio_clips[index].end = audio_clips[index].start + 512;
        }
        else
        {
            audio_clips[index].end -= number_of_blocks * 512;
        }
    }
}