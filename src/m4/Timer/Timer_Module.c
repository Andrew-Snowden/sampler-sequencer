#include "Timer_Module.h"

#include "myprint.h"
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

	//HAL_TIM_Base_Init(&htim6);		// Configure the timer
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
		print_char_nl('a');

	if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
		print_char_nl('a');

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
		print_char_nl('c');

	print_char_nl('1');
}

void Init_TIM7(void)
{

}

void TIM6_IRQHandler(void)
{
	Trellis_Read_Buttons();
	HAL_TIM_IRQHandler(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {}
