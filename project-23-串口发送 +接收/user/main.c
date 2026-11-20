#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "serial.h"

uint8_t RxData;

int main(void)
{
	OLED_Init();
	Serial_Init();
		
	OLED_ShowString(1,1,"RxData:");

	while (1) 
    {
		if (Serial_GetRxFlag()==1)
		{
			RxData = SerialGetData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1,8,RxData,4);
		}
	}
}
