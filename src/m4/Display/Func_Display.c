#include "Func_Display.h"

#include "trellis_board.h"

static struct TrellisBoard funcBoardRefrence;
struct TrellisBoard* funcBoard = NULL;

void Func_Display_Init(void)
{
	funcBoardRefrence.index = 1;
	funcBoardRefrence.leds = 0x0000;
	funcBoardRefrence.btns = 0x0000;
	funcBoardRefrence.prevBtns = 0x0000;
	TrellisBoard_Init(&funcBoardRefrence);
	funcBoard = &funcBoardRefrence;
}

uint8_t Func_Display_Read_Button(uint8_t button)
{
	if (funcBoard == NULL)
		return 0;
	else if (button < 0 || button > 15)
		return 0;
	else if (funcBoard->btns & ~(funcBoard->prevBtns) & (1 << button))
	{
		funcBoard->prevBtns |= (1 << button);
		return 2;
	}
	else if (funcBoard->btns & (1 << button))
	{
		funcBoard->prevBtns |= (1 << button);
		return 1;
	}
	else
	{
		funcBoard->prevBtns &= ~(1 << button);
		return 0;
	}
}

