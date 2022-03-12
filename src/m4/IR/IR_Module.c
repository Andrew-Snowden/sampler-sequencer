#include "IR_Module.h"

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"
#include "Timer_Module.h"
#include "Audio_Processor.h"
#include "myprint.h"
#include <stdlib.h>

#define DEBUG_PRINT_IR_COMMAND 0

static void IR_Callback(void);

void IR_Init(void)
{
    HAL_NVIC_EnableIRQ(EXTI13_IRQn);
}

void EXTI13_IRQHandler(void)
{
    IR_Callback();
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
}

void IR_Callback(void)
{
    static uint32_t prevData = 0xFFFFFFFF;
    uint32_t data = 0;
    while (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))); // Wait for 9ms pulse low
    while ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))); // Wait for 4.5 pulse high
    for (uint8_t i = 0; i < 32; i++) {
        uint8_t count = 0;
        while (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))); // Wait for 562.5µs pulse low
        while ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))) // Read data
        {
            if (++count >= 20) // Abort if waited too long
                break;
            delay_us(100);
        }
        if (count >= 20) // Abort if waited too long
            break;
        else if (count > 12) // Pulse was a 1 if longer than 1.2ms
            data |= 1 << i;
    }
    if (DEBUG_PRINT_IR_COMMAND)
    {
        char str[9] = { '\0' };
        itoa(data, str, 16);
        print_string(str, 9);
        print_char_nl('\0');
    }
    if (data == 0) // Use previous data if data is 0 (repeat code), otherwise save
        data = prevData;
    else
        prevData = data;
    switch (data) {
    case 0xBA45FF00: // Power button
        break;
    case 0xB847FF00: // Func button
        break;
    case 0xB946FF00: // Vol up
        Audio_Processor_Modify_Volume(.1, 1);
        break;
    case 0xEA15FF00: // Vol down
        Audio_Processor_Modify_Volume(.1, 0);
        break;
    case 0xBF40FF00: // Play button
        break;
    case 0xBB44FF00: // Prev button
        break;
    case 0xBC43FF00: // Next button
        break;
    case 0xF609FF00: // Up button
        break;
    case 0xF807FF00: // Down button
        break;
    case 0xE619FF00: // EQ Button
        break;
    case 0xF20DFF00: // Rept Button
        break;
    case 0xE916FF00: // 0
        break;
    case 0xF30CFF00: // 1
        break;
    case 0xE718FF00: // 2
        break;
    case 0xA15EFF00: // 3
        break;
    case 0xF708FF00: // 4
        break;
    case 0xE31CFF00: // 5
        break;
    case 0xA55AFF00: // 6
        break;
    case 0xBD42FF00: // 7
        break;
    case 0xAD52FF00: // 8
        break;
    case 0xB54AFF00: // 9
        break;
    default:
        prevData = 0xFFFFFFFF; // Unset prevData since invalid code
        break;
    }
}