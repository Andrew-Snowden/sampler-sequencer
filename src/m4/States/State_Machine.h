#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

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
    DELETE_OPERATION,
    SELECT_OPERATION
} Operations;

typedef struct StateMachine
{
    States current_state;
    Operations operation;
    uint8_t source_index;
    uint8_t destination_index;
} StateMachine;

void State_Machine_Run();

void State_Machine_Set_State(States next_state);
void State_Machine_Set_Operation(Operations operation);

States State_Machine_Get_State();


#endif