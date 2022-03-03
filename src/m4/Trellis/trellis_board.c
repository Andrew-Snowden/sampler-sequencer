/*
 * trellis_board.c
 *
 *  Created on: Feb 8, 2022
 *      Author: Donald
 */


#include "trellis_board.h"
#include <stdlib.h>
#include <string.h>
#include "Periph_Init.h"

I2C_HandleTypeDef* hi2c1;

const uint8_t ledBytes[] = {
		0x7, 0x6, 0x6, 0x6,
		0x5, 0x5, 0x4, 0x4,
		0x2, 0x3, 0x2, 0x2,
		0x1, 0x1, 0x1, 0x0
};
const uint8_t ledBits[] = {
		0x04, 0x80, 0x20, 0x10,
		0x01, 0x02, 0x08, 0x10,
		0x40, 0x08, 0x02, 0x01,
		0x40, 0x20, 0x10, 0x04
};
const uint8_t btnBytes[] = {
		0x0, 0x0, 0x0, 0x2,
		0x0, 0x0, 0x0, 0x0,
		0x0, 0x1, 0x3, 0x2,
		0x1, 0x1, 0x1, 0x3
};
const uint8_t btnBits[] = {
		0x80, 0x10, 0x04, 0x04,
		0x20, 0x40, 0x01, 0x02,
		0x08, 0x01, 0x01, 0x02,
		0x08, 0x04, 0x02, 0x02
};

// Public functions
struct TrellisBoard* TrellisBoard_New(uint8_t index)
{
	if (index < 0 || index > 7)
		return NULL;
	struct TrellisBoard* board = malloc(sizeof(struct TrellisBoard));
	board->index = index;
	board->leds = 0x0000;
	board->btns = 0x0000;
	board->prevBtns = 0x0000;
	TrellisBoard_Init(board);
	return board;
}
struct TrellisBoard* TrellisBoard_Copy(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		struct TrellisBoard* newBoard = malloc(sizeof(struct TrellisBoard));
		memcpy(newBoard, board, sizeof(struct TrellisBoard));
		return newBoard;
	}
	return NULL;
}
void TrellisBoard_Delete(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		free(board);
		board = NULL;
	}
}
void TrellisBoard_Init(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		// Initialize I2C
		__TrellisBoard_I2CInit();

		// Turn on oscillator
		__TrellisBoard_TX(board->index, 0b00100001, NULL, 0);

		// Set interrupt pin to be active low
		__TrellisBoard_TX(board->index, 0b10100001, NULL, 0);

		// Set LEDs to expected state
		TrellisBoard_SendLEDs(board);

		// Set default brightness
		TrellisBoard_SetBrightness(board, 8);

		// Turn on display
		__TrellisBoard_TX(board->index, 0b10000001, NULL, 0);
	}
}
uint16_t TrellisBoard_SetLEDs(struct TrellisBoard* board, uint16_t ledMask)
{
	if (board != NULL)
	{
		board->leds |= ledMask;
		return board->leds;
	}
	return 0;
}
uint16_t TrellisBoard_ClrLEDs(struct TrellisBoard* board, uint16_t ledMask)
{
	if (board != NULL)
	{
		board->leds &= ~ledMask;
		return board->leds;
	}
	return 0;
}
uint16_t TrellisBoard_TglLEDs(struct TrellisBoard* board, uint16_t ledMask)
{
	if (board != NULL)
	{
		board->leds ^= ledMask;
		return board->leds;
	}
	return 0;
}
void TrellisBoard_SendLEDs(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		uint8_t leds[8];
		memset(leds, 0x0, 8);
		for (uint8_t i=0; i<16; i++)
		{
			if (board->leds & (1 << i))
				leds[ledBytes[i]] |= ledBits[i];
		}
		__TrellisBoard_TX(board->index, 0b00000000, leds, 8);
	}
}
uint16_t TrellisBoard_ReadBtns(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		uint8_t btns[6];
		__TrellisBoard_RX(board->index, 0b01000000, btns, 6);
		//board->prevBtns = board->btns;
		board->btns = 0x0000;
		for (uint8_t i=0; i<16; i++)
		{
			if (btns[btnBytes[i]] & btnBits[i])
				board->btns |= 1 << i;
		}
		return board->btns;
	}
	return 0;
}
uint16_t TrellisBoard_NonReadBtns(struct TrellisBoard* board)
{
	if (board != NULL)
	{
		//board->prevBtns = board->btns;
		board->btns = 0x0000;
	}
	return 0;
}
void TrellisBoard_SetBlinking(struct TrellisBoard* board, uint8_t freq)
{
	if (board != NULL)
	{
		if (freq < 0)
			freq = 0;
		else if (freq > 3)
			freq = 3;
		__TrellisBoard_TX(board->index, 0b10000001 | (freq << 1), NULL, 0);
	}
}
void TrellisBoard_SetBrightness(struct TrellisBoard* board, uint8_t brighness)
{
	if (board != NULL)
	{
		if (brighness < 1)
			brighness = 1;
		else if (brighness > 16)
			brighness = 16;
		__TrellisBoard_TX(board->index, 0b11100000 | brighness, NULL, 0);
	}
}

// Private Functions
void __TrellisBoard_I2CInit()
{
	static uint8_t i2cInitialized = 0;
	if (i2cInitialized == 0)
	{
		hi2c1 = Handle_Get_I2C1();
		i2cInitialized = 1;
	}
}
void __TrellisBoard_TX(uint8_t boardIndex, uint8_t command, uint8_t* data, uint8_t dataLen)
{
	if (boardIndex < 0 || boardIndex > 7)
		return;
	uint8_t txBuffer[9];
	txBuffer[0] = command;
	if (data != NULL && dataLen > 0)
		memcpy(txBuffer+1, data, dataLen);
	HAL_I2C_Master_Transmit(hi2c1, (0x70 + boardIndex) << 1, txBuffer, dataLen + 1, HAL_MAX_DELAY);
}
void __TrellisBoard_RX(uint8_t boardIndex, uint8_t command, uint8_t* buffer, uint8_t bufferLen)
{
	if (boardIndex < 0 || boardIndex > 7)
		return;
	HAL_I2C_Master_Transmit(hi2c1, (0x70 + boardIndex) << 1, &command, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(hi2c1, (0x70 + boardIndex) << 1, buffer, bufferLen, HAL_MAX_DELAY);
}
