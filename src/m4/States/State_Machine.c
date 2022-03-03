#include "State_Machine.h"

#include "Audio_Processor.h"
#include "State_Functions.h"

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
        {
            Play_Mode_Function();
        }
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
    Default_Play_Mode();
    //Read Buttons and operate based on that

    //If audio button is pressed (and allocated), audio is added to Audio Processor queue

    //If select button is pressed, switch to select mode state
}

static void Select_Mode_Function()
{
    //Flash available pads

    //If user makes valid selection, flash the available operations/functions

    //If user selects valid operation, set the operation in state_machine and switch state to operation mode
}

static void Operation_Mode_Function()
{
    //Based on state_machine.operation, do the correct functionality

    //When operation is complete, go back to Play Mode state
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