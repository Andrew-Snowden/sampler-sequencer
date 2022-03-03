#include "Trellis_Read_Buttons.h"

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"
#include "trellis_board.h"

extern struct TrellisBoard* audioBoard;
extern struct TrellisBoard* funcBoard;

void Trellis_Read_Buttons(void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	{
		TrellisBoard_ReadBtns(audioBoard);
		TrellisBoard_ReadBtns(funcBoard);
	}
	else
	{
		TrellisBoard_NonReadBtns(audioBoard);
		TrellisBoard_NonReadBtns(funcBoard);
	}
}