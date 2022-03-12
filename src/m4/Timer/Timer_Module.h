#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

void init_Timers(void);
void Init_TIM6(void);
void Init_TIM7(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void delay_us(uint16_t us);

#endif