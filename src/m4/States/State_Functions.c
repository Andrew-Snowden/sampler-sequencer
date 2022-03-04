#include "State_Functions.h"
#include "Audio_Processor.h"
#include "Audio_Display.h"
#include "Func_Display.h"
#include "myprint.h"

void Default_Play_Mode()
{
    Audio_Processor_Run();

}

uint8_t Read_Allocated_Button_Press()
{
    uint8_t selection_made = 0;
    uint8_t result = 0;

    while (selection_made == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (Audio_Display_Read_Button(i) == 2 && Audio_Is_Slot_Free(i) == 0 && selection_made == 0)
            {
                selection_made = 1;
                result = i;
            }
        }
    }

    return result;
}

uint8_t Read_Unallocated_Button_Press()
{
    uint8_t selection_made = 0;
    uint8_t result = 0;

    while (selection_made == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (Audio_Display_Read_Button(i) == 2 && Audio_Is_Slot_Free(i) == 1 && selection_made == 0)
            {
                selection_made = 1;
                result = i;
            }
        }
    }

    return result;
}

uint8_t Read_Audio_Button_Press()
{
    uint8_t selection_made = 0;
    uint8_t result = 0;

    while (selection_made == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (Audio_Display_Read_Button(i) == 2 && selection_made == 0)
            {
                selection_made = 1;
                result = i;
            }
        }
    }

    return result;
}


void Default_Play_Mode_Read_Audio_Buttons(void)
{
    for (int i = 0; i < 16; i++)
    {
        if (Audio_Display_Read_Button(i) == 2 && Audio_Is_Slot_Free(i) == 0)
        {
            if (Audio_Processor_Is_Clip_Queued(i))
            {
                if (Audio_Get_Clip(i)->is_repeating)
                {
                    Audio_Processor_Remove_Clip(i);
                }
                else
                {
                    Audio_Processor_Remove_Clip(i);
                    Audio_Processor_Add_Clip(i);
                }
                print_char_nl('8');
            }
            else
            {
                print_char_nl('9');
                Audio_Processor_Add_Clip(i);
            }
        }
        else if (Audio_Display_Read_Button(i) == 0 && Audio_Processor_Is_Clip_Queued(i) == 1)
        {
            if (Audio_Get_Clip(i)->play_through == 0)
            {
                Audio_Processor_Remove_Clip(i);
                print_char_nl('7');
            }
        }
        else if (Audio_Display_Read_Button(i) == 1 && Audio_Is_Slot_Free(i) == 0)
        {
            //print_char_nl('3');
        }

        //print_char_nl(Audio_Processor_Is_Clip_Queued(1) + 48);
    }
}

void Select_Play_Mode_Read_Buttons(uint8_t clip_index)
{
    if (Func_Display_Read_Button(SAMPLE_FUNC) == 2)
    {
        Audio_Processor_Add_Clip(clip_index);
    }
    else if (Func_Display_Read_Button(SAMPLE_FUNC) == 0 && Audio_Processor_Is_Clip_Queued(clip_index) == 1)
    {
        Audio_Processor_Remove_Clip(clip_index);
    }
}

void Sample_Function(uint8_t pad_index)
{
    //Start sampling onto pad_index
    Audio_Processor_Sample_Start();

    uint8_t stopped = 0;
    //If user presses sample button again, stop sampling
    while(Audio_Processor_Get_Receive_Status() == RECEIVE_BUSY)
    {
        if (Func_Display_Read_Button(SAMPLE_FUNC) == 2)
        {
            Audio_Processor_Sample_Stop(pad_index);
            stopped = 1;
        }
    }

    if (stopped == 0)
    {
        Audio_Processor_Sample_Stop(pad_index);
    }
    

    //Display allocated pads
}

void Copy_Function(uint8_t source_index)
{
    Audio_Display_Flash_Available();

    uint8_t destination_index = Read_Unallocated_Button_Press();

    Audio_Clip_Copy(destination_index, source_index);
}

void Delete_Function(uint8_t source_index)
{
    Audio_Clip_Delete(source_index);
}

void Move_Function(uint8_t source_index)
{
    Audio_Display_Flash_Available();

    uint8_t destination_index = Read_Unallocated_Button_Press();

    Audio_Clip_Move(destination_index, source_index);
}