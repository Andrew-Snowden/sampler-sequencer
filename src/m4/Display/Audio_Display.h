#ifndef AUDIO_DISPLAY_H
#define AUDIO_DISPLAY_H

#include <stdint.h>

void Audio_Display_Init(void);
void Audio_Display_Flash_Available(void);
void Audio_Display_Show_Allocated(void);
void Audio_Display_Off(void);
uint8_t Audio_Display_Read_Button(uint8_t button);

#endif