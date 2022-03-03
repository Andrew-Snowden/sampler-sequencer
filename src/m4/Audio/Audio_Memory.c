#include "Audio_Memory.h"

#include <stdint.h>
#include <string.h>

#include "defines.h"

static AudioClip audio_clips[17];

void Audio_Clip_Copy(uint8_t index_destination, uint8_t index_source)
{
    //Audio_Clip_Load(index_destination, audio_clips[index_source].audio, audio_clips[index_source].length_32);

    AudioClip *destination = &audio_clips[index_destination];
    AudioClip *source = &audio_clips[index_source];

    memcpy(destination->audio, source->audio, source->length_32 * sizeof(int32_t));
    destination->length_32 = source->length_32;
    destination->start = destination->audio;
    destination->end = destination->audio + destination->length_32;
    destination->volume = source->volume;
    
    destination->read_ptr = destination->start;
    destination->is_allocated = 1;
}

StatusType Audio_Clip_Load(uint8_t index_destination, int32_t *source_address, int32_t number_of_samples)
{
    //Load
    if (Audio_Is_Slot_Free(index_destination))
    {
        Audio_Clip_Delete(index_destination);
        memcpy(audio_clips[index_destination].audio, source_address, number_of_samples * sizeof(int32_t));

        //Set audio clip parameters
        audio_clips[index_destination].length_32    = number_of_samples;
        audio_clips[index_destination].end          = audio_clips[index_destination].audio + number_of_samples;
        audio_clips[index_destination].start        = audio_clips[index_destination].audio;
        audio_clips[index_destination].read_ptr     = audio_clips[index_destination].start;
        audio_clips[index_destination].volume       = 1.0f;


        //Set allocate bit
        audio_clips[index_destination].is_allocated = 1;
    }
    else
    {
        return Status_Fail;
    }
}

StatusType Audio_Load_Buffer(int32_t *source, int32_t number_of_samples, int32_t sample_offset)
{
    if ((audio_clips[16].audio + sample_offset + number_of_samples) >= audio_clips[16].audio + 960000)
    {
        return Status_Fail;
    }
    else 
    {
        memcpy(audio_clips[16].audio + sample_offset, source, number_of_samples * 4);
    }
    return Status_Success;
}

void Audio_Clip_Delete(uint8_t index)
{
    memset(audio_clips[index].audio, 0, 960000 * sizeof(int32_t));

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

AudioClip *Audio_Get_Clip(uint8_t index)
{
    return &audio_clips[index];
}

AudioClip *Audio_Get_Buffer()
{
    return &audio_clips[16];
}

uint8_t Audio_Get_Buffer_Index()
{
    return 16;
}

void Audio_Clip_Set_Repeating(uint8_t index, uint8_t is_repeating)
{
    audio_clips[index].is_repeating = is_repeating;
}

void Audio_Clip_Set_UseEffects(uint8_t index, uint8_t use_effects)
{
    audio_clips[index].use_effects = use_effects;
}

void Audio_Clip_Set_Playthrough(uint8_t index, uint8_t play_through)
{
    audio_clips[index].play_through = play_through;
}

void Audio_Clip_Set_Volume(uint8_t index, float volume)
{
    audio_clips[index].volume = volume;
}

void Audio_Clip_Reset_Start(uint8_t index)
{
    audio_clips[index].start = audio_clips[index].audio;
}

void Audio_Clip_Reset_End(uint8_t index)
{
    audio_clips[index].end = audio_clips[index].audio + audio_clips[index].length_32;
}

void Audio_Clip_Modify_Start(uint8_t index, int32_t number_of_blocks, ClipModifyDirection direction)
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
        if ((void*)audio_clips[index].audio > (void*)audio_clips[index].start - (number_of_blocks * 512))
        {
            audio_clips[index].start = audio_clips[index].audio;
        }
        else
        {
            audio_clips[index].start -= number_of_blocks * 512;
        }
    }

}

void Audio_Clip_Modify_End(uint8_t index, int32_t number_of_blocks, ClipModifyDirection direction)
{
    if (direction == FORWARD)
    {
        if (((void*)audio_clips[index].end + (number_of_blocks * 512)) > ((void*)audio_clips[index].audio + audio_clips[index].length_32))
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