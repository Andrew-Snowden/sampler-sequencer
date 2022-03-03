#include "State_Functions.h"
#include "Audio_Processor.h"
#include "Audio_Display.h"
#include "Func_Display.h"
#include "myprint.h"

void Default_Play_Mode()
{
    Audio_Processor_Run();

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
            if (Audio_Get_Clip(i)->is_repeating == 0 && Audio_Get_Clip(i)->play_through == 0)
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