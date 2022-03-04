#ifndef STATE_FUNCTIONS_H
#define STATE_FUNCTIONS_H

#include <stdint.h>

void Default_Play_Mode();
void Default_Play_Mode_Read_Audio_Buttons(void);
void Select_Play_Mode_Read_Buttons(uint8_t clip_index);
uint8_t Read_Allocated_Button_Press();
uint8_t Read_Unallocated_Button_Press();
uint8_t Read_Audio_Button_Press();

void Sample_Function(uint8_t pad_index);
void Copy_Function(uint8_t source_index);
void Delete_Function(uint8_t source_index);
void Move_Function(uint8_t source_index);


#endif