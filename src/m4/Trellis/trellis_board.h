/*
 * trellis_board.h
 *
 *  Created on: Feb 8, 2022
 *      Author: Donald
 */

#ifndef INC_TRELLIS_BOARD_H_
#define INC_TRELLIS_BOARD_H_

#include "stm32mp1xx_hal.h"

struct TrellisBoard {
	uint8_t index;
	uint16_t leds;
	uint16_t btns;
	uint16_t prevBtns;
};

// Public functions
struct TrellisBoard* TrellisBoard_New(uint8_t index); // Only accepts 0-7 (returns NULL if out of bounds)
struct TrellisBoard* TrellisBoard_Copy(struct TrellisBoard* board);
void TrellisBoard_Delete(struct TrellisBoard* board);
void TrellisBoard_Init(struct TrellisBoard* board);
uint16_t TrellisBoard_SetLEDs(struct TrellisBoard* board, uint16_t ledMask);
uint16_t TrellisBoard_ClrLEDs(struct TrellisBoard* board, uint16_t ledMask);
uint16_t TrellisBoard_TglLEDs(struct TrellisBoard* board, uint16_t ledMask);
void TrellisBoard_SendLEDs(struct TrellisBoard* board);
uint16_t TrellisBoard_ReadBtns(struct TrellisBoard* board);
uint16_t TrellisBoard_NonReadBtns(struct TrellisBoard* board);
void TrellisBoard_SetBlinking(struct TrellisBoard* board, uint8_t freq); // Only accepts 0-3 [0Hz, 2Hz, 1Hz, 0.5Hz] (rounds if out of bounds)
void TrellisBoard_SetBrightness(struct TrellisBoard* board, uint8_t brighness); // Only accepts 1-16 (rounds if out of bounds)

// Private Functions
void __TrellisBoard_I2CInit();
void __TrellisBoard_TX(uint8_t boardIndex, uint8_t command, uint8_t* data, uint8_t dataLen);
void __TrellisBoard_RX(uint8_t boardIndex, uint8_t command, uint8_t* buffer, uint8_t bufferLen);


#endif /* INC_TRELLIS_BOARD_H_ */
