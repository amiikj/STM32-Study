#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "serial.h"


int main(void)
{
	OLED_Init();
	Serial_Init();
	Serial_SendByte(0x41);
	
///	uint16_t MyArray[] = {0x42,0x43,0x44,0x45};
///	Serial_SendArray(MyArray,4);
	
///	Serial_SendString("HELLO WORLD!");
///	Serial_SendNumber(123456,6);
	printf("Num=%d\r\n",666);

	while (1) 
    {
		
	}
}
