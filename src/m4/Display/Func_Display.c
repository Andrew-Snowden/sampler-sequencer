#include "Func_Display.h"

#include "trellis_board.h"
#include "defines.h"

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

void Func_Display_Flash_NotAllocated(void)
{
	TrellisBoard_ClrLEDs(funcBoard, 0xFFFF);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << SAMPLE_FUNC);
	TrellisBoard_SetBrightness(funcBoard, 8);
	TrellisBoard_SendLEDs(funcBoard);
	TrellisBoard_SetBlinking(funcBoard, 1);
}

void Func_Display_Show_NotAllocated(void)
{
	TrellisBoard_ClrLEDs(funcBoard, 0xFFFF);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << SAMPLE_FUNC);
	TrellisBoard_SetBrightness(funcBoard, 8);
	TrellisBoard_SendLEDs(funcBoard);
	TrellisBoard_SetBlinking(funcBoard, 0);
}

void Func_Display_Flash_Allocated(void)
{
	TrellisBoard_ClrLEDs(funcBoard, 0xFFFF);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << SELECT_FUNC);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << COPY_FUNC);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << DELETE_FUNC);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << MOVE_FUNC);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << REPEATING_FUNC);
	TrellisBoard_SetLEDs(funcBoard, 0x1 << PLAYTHROUGH_FUNC);
	TrellisBoard_SetBrightness(funcBoard, 8);
	TrellisBoard_SendLEDs(funcBoard);
	TrellisBoard_SetBlinking(funcBoard, 1);
}

void Func_Display_Clear()
{
	TrellisBoard_ClrLEDs(funcBoard, 0xFFFF);
	TrellisBoard_SendLEDs(funcBoard);
}