#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "timber.h"

uint16_t Num;

int main(void)
{
	Timber_Init();
	OLED_Init();
	
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(2,1,"Count:");
	
	while (1) 
    {
		OLED_ShowNum(1,5,Num,5);
		OLED_ShowNum(2,7,TIM_GetCounter(TIM2),5);
	}
}
