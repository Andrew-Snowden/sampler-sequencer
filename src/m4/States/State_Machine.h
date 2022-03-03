#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum States
{
    PLAY_MODE,
    SELECT_MODE,
    OPERATION_MODE
} States;

typedef enum Operations
{
    NO_OPERATION,
    SAMPLE_OPERATION,
    COPY_OPERATION,
    MOVE_OPERATION,
    DELETE_OPERATION
} Operations;

typedef struct StateMachine
{
    States current_state;
    Operations operation;
} StateMachine;

void State_Machine_Run();

void State_Machine_Set_State(States next_state);
void State_Machine_Set_Operation(Operations operation);

States State_Machine_Get_State();


#endif