#ifndef DEFINES_H
#define DEFINES_H

#define BLOCK_SIZE 	512
#define VOICES		8

typedef enum StatusType
{
    Status_Success,
    Status_Fail
} StatusType;

typedef enum AudioButtons
{
    BUTTON_0 = 0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_10,
    BUTTON_11,
    BUTTON_12,
    BUTTON_13,
    BUTTON_14,
    BUTTON_15
} AudioButtons;

typedef enum FunctionButtons
{
    SELECT_FUNC = 0,
    COPY_FUNC = 1,
    DELETE_FUNC = 2,
    SAMPLE_FUNC = 3,
    MOVE_FUNC = 4
} FunctionButtons;

#endif