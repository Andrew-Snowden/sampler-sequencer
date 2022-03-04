#ifndef ROTARY_MODULE_H
#define ROTARY_MODULE_H

#include <stdint.h>

typedef enum Granularity
{
    COARSE_ADJUST,
    FINE_ADJUST
} Granularity;

typedef enum Parameter
{
    VOLUME_PARAM,
    START_PARAM,
    END_PARAM,
    MASTER_VOLUME_PARAM,
    NO_PARAM
} Parameter;

typedef struct RotaryEncoder
{
    Granularity adjust;
    Parameter parameter;
    uint8_t clip_index;
    uint8_t associated;     //Indicates if it has any data associated with it
} RotaryEncoder;

void Rotary_Init();

void Rotary_Register_Item(uint8_t item_index, uint8_t rotary_number, Parameter param);

void Rotary_Unregister(uint8_t rotary_number);

void Rotary_Toggle_Granularity();


#endif