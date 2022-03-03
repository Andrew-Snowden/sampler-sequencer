#include "Audio_Display.h"

#include "trellis_board.h"
#include "Audio_Memory.h"

static struct TrellisBoard audioBoardRefrence;
struct TrellisBoard* audioBoard = NULL;

void Audio_Display_Init(void)
{
	audioBoardRefrence.index = 0;
	audioBoardRefrence.leds = 0x0000;
	audioBoardRefrence.btns = 0x0000;
	audioBoardRefrence.prevBtns = 0x0000;
	TrellisBoard_Init(&audioBoardRefrence);
	audioBoard = &audioBoardRefrence;
}

void Audio_Display_Flash_Available(void)
{
	TrellisBoard_ClrLEDs(audioBoard, 0xFFFF);
	for (uint8_t i = 0; i < 16; i++)
	{
		TrellisBoard_SetLEDs(audioBoard, Audio_Is_Slot_Free(i) << i);
	}
	TrellisBoard_SetBrightness(audioBoard, 8);
	TrellisBoard_SendLEDs(audioBoard);
	TrellisBoard_SetBlinking(audioBoard, 1);
}

void Audio_Display_Show_Allocated(void)
{
	TrellisBoard_SetLEDs(audioBoard, 0xFFFF);
	for (uint8_t i = 0; i < 16; i++)
	{
		TrellisBoard_ClrLEDs(audioBoard, Audio_Is_Slot_Free(i) << i);
	}
	TrellisBoard_SetBrightness(audioBoard, 2);
	TrellisBoard_SendLEDs(audioBoard);
	TrellisBoard_SetBlinking(audioBoard, 0);
}

void Audio_Display_Off(void)
{
	TrellisBoard_ClrLEDs(audioBoard, 0xFFFF);
	TrellisBoard_SendLEDs(audioBoard);
	TrellisBoard_SetBlinking(audioBoard, 0);
}

uint8_t Audio_Display_Read_Button(uint8_t button)
{
	if (audioBoard == NULL)
		return 0;
	else if (button < 0 || button > 15)
		return 0;
	else if (audioBoard->btns & ~(audioBoard->prevBtns) & (1 << button))
	{
		audioBoard->prevBtns |= (1 << button);
		return 2;
	}
	else if (audioBoard->btns & (1 << button))
	{
		audioBoard->prevBtns |= (1 << button);
		return 1;
	}
	else
	{
		audioBoard->prevBtns &= ~(1 << button);
		return 0;
	}
}

