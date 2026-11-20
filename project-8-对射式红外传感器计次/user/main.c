#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "countsensor.h"

int main(void)
{
	OLED_Init();
	Init_Countsensor();
	while (1) 
    {

		OLED_ShowString(1,1,"Count:");
		OLED_ShowNum(1,7,Countsensor_Get(),5); /*为什么长度要设为5？*/
	}
}
