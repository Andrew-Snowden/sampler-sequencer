#ifndef FUNC_DISPLAY_H
#define FUNC_DISPLAY_H

#include <stdint.h>

void Func_Display_Init(void);
void Func_Display_Flash_Allocated(void);
void Func_Display_Flash_NotAllocated(void);
uint8_t Func_Display_Read_Button(uint8_t button);
void Func_Display_Clear();
void Func_Display_Show_NotAllocated(void);

#endif