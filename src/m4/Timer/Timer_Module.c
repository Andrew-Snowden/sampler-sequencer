#include "Timer_Module.h"

#include "myprint.h"
#include <stdlib.h>
#include <string.h>
#include "Trellis_Read_Buttons.h"

TIM_HandleTypeDef htim6; // Trellis timer
TIM_HandleTypeDef htim7; // IR timer

void init_Timers(void)
{
	Init_TIM6();
	Init_TIM7();
}

void Init_TIM6(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 10440-1;	// 208.8MHz / 800 = 20kHz
	htim6.Init.Period = 1000-1;		// 20kHz / 1000 = 20Hz = 50ms
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	__HAL_RCC_TIM6_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);

	HAL_TIM_Base_Init(&htim6);		// Configure the timer

	HAL_TIM_Base_Start_IT(&htim6);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);
}

void Init_TIM7(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 209 - 1;	// 208.8MHz / 209 = 0.99904MHz
	htim7.Init.Period = 0xFFFF - 1;	// Set to max
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	__HAL_RCC_TIM7_CLK_ENABLE();

	HAL_TIM_Base_Init(&htim7);		// Configure the timer
	HAL_TIM_Base_Start(&htim7);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);
}

void TIM6_IRQHandler(void)
{
	Trellis_Read_Buttons();
	HAL_TIM_IRQHandler(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {}

void delay_us(uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim7, 0);
	while (__HAL_TIM_GET_COUNTER(&htim7) < us);
}
