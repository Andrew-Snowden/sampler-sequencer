#include "State_Machine.h"

#include "Audio_Processor.h"
#include "Audio_Memory.h"
#include "State_Functions.h"
#include "Func_Display.h"
#include "Audio_Display.h"
#include "defines.h"
#include "myprint.h"
#include "Rotary_Module.h"

//Include State functions

static StateMachine state_machine = {PLAY_MODE, NO_OPERATION};
static void Play_Mode_Function();
static void Select_Mode_Function();
static void Operation_Mode_Function();

void State_Machine_Run()
{
    switch(State_Machine_Get_State())
    {
        case PLAY_MODE:
            Play_Mode_Function();
        break;

        case SELECT_MODE:
            Select_Mode_Function();
        break;

        case OPERATION_MODE:
            Operation_Mode_Function();
        break;
    }
}

static void Play_Mode_Function()
{
    Default_Play_Mode_Read_Audio_Buttons();

    //print_char_nl(Audio_Get_Clip(0)->is_repeating + 48);

    Default_Play_Mode();

    //Read function buttons
    if (Func_Display_Read_Button(SELECT_FUNC) == 2)
    {
        //Stop audio processing/output
        Audio_Processor_Stop();

        //switch state to select mode
        state_machine.current_state = SELECT_MODE;
    }

}

static void Select_Mode_Function()
{
    //Flash available pads
    Audio_Display_Show_Allocated();

    //Read user selection and flash the available operations/functions
    uint8_t selection_made = 0;
    uint8_t selection_index = 0;
    uint8_t is_not_allocated = 0;

    while(selection_made == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (selection_made == 0 && Audio_Display_Read_Button(i) == 2)
            {
                selection_made = 1;
                is_not_allocated = Audio_Is_Slot_Free(i);
                state_machine.source_index = i;
            }
        }
    }

    selection_made = 0;
    selection_index = 0;

    if (is_not_allocated)   //Flash sample button
    {
        Func_Display_Flash_NotAllocated();
    }
    else    //Flash operations for allocated audio clip operations
    {
        Func_Display_Flash_Allocated();
        Rotary_Register_Item(state_machine.source_index, 0, START_PARAM);
        Rotary_Register_Item(state_machine.source_index, 1, END_PARAM);
        //Rotary_Register_Item(state_machine.source_index, 2, VOLUME_PARAM);
    }


    //Read operation selection
    while(selection_made == 0)
    {
        Select_Play_Mode_Read_Buttons(state_machine.source_index);

        Default_Play_Mode();



        for (int i = 0; i < 16; i++)
        {
            if (selection_made == 0 && Func_Display_Read_Button(i) == 2)
            {
                selection_made = 1;
                selection_index = i;
            }
        }
        if (selection_made)
        {
            if (Audio_Processor_Is_Clip_Queued(state_machine.source_index) == 1)
            {
                Audio_Processor_Remove_Clip(state_machine.source_index);
            }

            if (is_not_allocated && selection_index == SAMPLE_FUNC) //Run sample operation
            {
                state_machine.operation = SAMPLE_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (is_not_allocated == 0 && selection_index == DELETE_FUNC)   //Run delete operation
            {
                state_machine.operation = DELETE_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (is_not_allocated == 0 && selection_index == COPY_FUNC) //Run copy operation
            {
                state_machine.operation = COPY_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (is_not_allocated == 0 && selection_index == MOVE_FUNC) //Run copy operation
            {
                state_machine.operation = MOVE_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (selection_index == SELECT_FUNC)
            {
                state_machine.operation = SELECT_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (is_not_allocated == 0 && selection_index == REPEATING_FUNC)
            {
                Audio_Clip_Toggle_Repeating(state_machine.source_index);
                state_machine.operation = SELECT_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else if (is_not_allocated == 0 && selection_index == PLAYTHROUGH_FUNC)
            {
                if (Audio_Get_Clip(state_machine.source_index)->play_through == 0)
                {
                    Audio_Clip_Set_Playthrough(state_machine.source_index, 1);
                }
                else
                {
                    Audio_Clip_Set_Playthrough(state_machine.source_index, 0);
                }
                state_machine.operation = SELECT_OPERATION;
                state_machine.current_state = OPERATION_MODE;
            }
            else
            {
                selection_made = 0;
            }
        }
    }

    Rotary_Unregister(0);
    Rotary_Unregister(1);
    //Rotary_Unregister(2);
}

static void Operation_Mode_Function()
{
    //Based on state_machine.operation, do the correct functionality
    switch(state_machine.operation)
    {
        case SAMPLE_OPERATION:
            print_string("Sampling...\n", 12);
            Sample_Function(state_machine.source_index);
        break;

        case COPY_OPERATION:
            print_string("Copying...\n", 11);
            Copy_Function(state_machine.source_index);
        break;

        case MOVE_OPERATION:
            print_string("Moving...\n", 10);
            Move_Function(state_machine.source_index);
        break;

        case DELETE_OPERATION:
            print_string("Deleting...\n", 12);
            Delete_Function(state_machine.source_index);
        break;

        case SELECT_OPERATION:

        break;
    }


    //When operation is complete, go back to Play Mode state
    state_machine.operation = NO_OPERATION;
    state_machine.current_state = PLAY_MODE;
    Audio_Display_Show_Allocated();
    Func_Display_Clear();
    Audio_Processor_Start();
}

void State_Machine_Set_State(States next_state)
{
    switch (state_machine.current_state)
    {
        case PLAY_MODE:
        {
            if (next_state != PLAY_MODE)
            {
                Audio_Processor_Stop();
                state_machine.current_state = next_state;
            }
        }
        break;

        case SELECT_MODE:

        break;

        case OPERATION_MODE:

        break;
    }
}

void State_Machine_Set_Operation(Operations operation)
{
    //Set operation to operation
    state_machine.operation = operation;
}

States State_Machine_Get_State()
{
    return state_machine.current_state;
}