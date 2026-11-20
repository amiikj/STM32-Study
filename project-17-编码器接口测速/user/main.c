#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "timber.h"
#include "encoder.h"
#include "Delay.h"

uint16_t Num;

int main(void)
{
	Timber_Init();
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"CNT:");
	
	while (1) 
    {
		OLED_ShowSignedNum(1,5,Encoder_Get(),5);
		Delay_ms(1000);
	}

}

//为什么CNT是四个四个增加？
//为什么该程序能能测速？