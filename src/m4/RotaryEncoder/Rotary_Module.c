#include "Rotary_Module.h"

#include <stdint.h>

#include "stm32mp1xx_hal.h"

#include "Audio_Memory.h"
#include "myprint.h"


static volatile RotaryEncoder rotary_encoders[4];
static volatile uint8_t entries = 0;

static void Rotary_Callback(uint8_t rotary_number, uint8_t direction);

void Rotary_Init()
{
    for (int i = 0; i < 4; i++)
    {
        rotary_encoders[i].adjust = COARSE_ADJUST;
        rotary_encoders[i].parameter = NO_PARAM;
        rotary_encoders[i].associated = 0;
        rotary_encoders[i].clip_index = 0;
    }

    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_EnableIRQ(EXTI5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI7_IRQn);
    HAL_NVIC_EnableIRQ(EXTI6_IRQn);
    HAL_NVIC_EnableIRQ(EXTI8_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_IRQn);
}

void Rotary_Register_Item(uint8_t item_index, uint8_t rotary_number, Parameter param)
{
    rotary_encoders[rotary_number].adjust = COARSE_ADJUST;
    rotary_encoders[rotary_number].parameter = param;
    rotary_encoders[rotary_number].clip_index = item_index;
    rotary_encoders[rotary_number].associated = 1;
}

void Rotary_Unregister(uint8_t rotary_number)
{
    rotary_encoders[rotary_number].associated = 0;
    rotary_encoders[rotary_number].parameter = NO_PARAM;
    rotary_encoders[rotary_number].adjust = COARSE_ADJUST;
    rotary_encoders[rotary_number].clip_index = 0;
}

void Rotary_Toggle_Granularity()
{
    if (rotary_encoders[0].adjust == COARSE_ADJUST)
    {
        for (int i = 0; i < 4; i++)
        {
            rotary_encoders[i].adjust = FINE_ADJUST;
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            rotary_encoders[i].adjust = COARSE_ADJUST;
        }
    }
}

static void Rotary_Callback(uint8_t rotary_number, uint8_t direction)
{
    if (rotary_encoders[rotary_number].associated == 1)
    {
        switch(rotary_encoders[rotary_number].parameter)
        {
            case VOLUME_PARAM:
            {
                if (rotary_encoders[rotary_number].adjust == COARSE_ADJUST)
                {
                    Audio_Clip_Adjust_Volume(rotary_encoders[rotary_number].clip_index, 0.1, direction);
                }
                else
                {
                    Audio_Clip_Modify_Start(rotary_encoders[rotary_number].clip_index, 0.01, direction);
                }
            }
            break;

            case START_PARAM:
            {
                if (rotary_encoders[rotary_number].adjust == COARSE_ADJUST)
                {
                    Audio_Clip_Modify_Start(rotary_encoders[rotary_number].clip_index, 8, direction);
                }
                else
                {
                    Audio_Clip_Modify_Start(rotary_encoders[rotary_number].clip_index, 1, direction);
                }
            }
            break;

            case END_PARAM:
            {
                if (rotary_encoders[rotary_number].adjust == COARSE_ADJUST)
                {
                    Audio_Clip_Modify_End(rotary_encoders[rotary_number].clip_index, 8, direction);
                }
                else
                {
                    Audio_Clip_Modify_End(rotary_encoders[rotary_number].clip_index, 1, direction);
                }
            }
            break;

            case MASTER_VOLUME_PARAM:

            break;

            case NO_PARAM:
            default:

            break;
        }
    }
}


//Interrupts
void EXTI6_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);

    Rotary_Callback(0, 0);
    print_string("R0 Down\n", 8);
    
}

void EXTI8_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);

    Rotary_Callback(0, 1);

    print_string("R0 Up\n", 6);
    
}

void EXTI15_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(1, 0);

        print_string("R1 Down\n", 8);

        entries = 0;
    }

    
}

void EXTI7_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(1, 1);

        print_string("R1 Up\n", 6);

        entries = 0;
    }

    
}

void EXTI0_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(2, 0);

        entries = 0;
    }

    
}

void EXTI1_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(2, 1);

        entries = 0;
    }

    
}

void EXTI5_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(3, 0);

        entries = 0;
    }

    
}

void EXTI4_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);

    entries++;

    if (entries >= 2)
    {
        Rotary_Callback(3, 1);

        entries = 0;
    }
    
}




