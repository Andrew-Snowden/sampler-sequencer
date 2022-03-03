#include "State_Machine.h"

#include "Audio_Processor.h"
#include "Audio_Memory.h"
#include "State_Functions.h"
#include "Func_Display.h"
#include "Audio_Display.h"
#include "defines.h"
#include "myprint.h"

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

    Default_Play_Mode();

    //Read function buttons
    if (Func_Display_Read_Button(SELECT_FUNC))
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
            if (selection_made == 0 && Audio_Display_Read_Button(i))
            {
                selection_made = 1;
                selection_index = i;
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
    }


    //Read operation selection
    while(selection_made == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (selection_made == 0 && Func_Display_Read_Button(i))
            {
                selection_made = 1;
                selection_index = i;
            }
        }
        if (selection_made)
        {
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
            else
            {
                selection_made = 0;
            }
        }
    }

    
}

static void Operation_Mode_Function()
{
    //Based on state_machine.operation, do the correct functionality
    switch(state_machine.operation)
    {
        case SAMPLE_OPERATION:
            print_string("Sampling...\n", 12);
        break;

        case COPY_OPERATION:

        break;

        case MOVE_OPERATION:

        break;

        case DELETE_OPERATION:

        break;
    }


    //When operation is complete, go back to Play Mode state
    state_machine.operation = NO_OPERATION;
    state_machine.current_state = PLAY_MODE;
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