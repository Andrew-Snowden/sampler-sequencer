#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

#include "stm32mp1xx.h"
#include "system_stm32mp1xx.h"
#include "stm32mp1xx_hal.h"

#include "Audio_Struct.h"



Audio * audio_def;

void EffectsProcessAndMix();
void MasterProcess(); //Final stage of audio processing. Audio transmitted to SAI via DMA. Ping-pong buffering. 512Byte block size

void ProcessAudio();




/* TODO: Array of audio effects to apply in MasterProcessing */

#endif