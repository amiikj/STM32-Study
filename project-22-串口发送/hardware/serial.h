#ifndef _SERIAL_H
#define _SERIAL_H
#include <stdio.h>

void Serial_Init(void);

void Serial_SendByte(uint16_t Byte);

void Serial_SendArray(uint16_t *Array,uint16_t Length);

void Serial_SendString(char *String);

void Serial_SendNumber(uint32_t Number,uint16_t Length);


#endif
