#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void Encoder_H4_Init(void);
int16_t Encoder_H4_GetDelta(void);

#endif
