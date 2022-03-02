#ifndef AUDIO_STRUCTS_H
#define AUDIO_STRUCTS_H

#include <stdint.h>
#include <string.h>

#include "stm32mp1xx_hal.h"

#include "defines.h"


typedef enum buffer_s
{
	BUFFER_1,
	BUFFER_2
} ActiveBuffer;

typedef enum status_s
{
	AUDIO_STATUS_READY,
	AUDIO_STATUS_BUSY
} AudioStatus;



#endif


























