#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "servo.h"
#include "Delay.h"
#include "key.h"

uint16_t KeyNum;
float Angle;




uint16_t a;
int main(void)
{
	OLED_Init();
	Servo_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"Angle:");
	while (1) 
    {
		
		
		KeyNum = Key_GetNum();
		if (KeyNum == 2)
		{
		 Angle +=30;
		
			if (Angle > 180)
			{
			Angle =0;
			}
		Delay_ms(100);
		Servo_SetAngle(Angle);
		}
		OLED_ShowNum(1,7,Angle,3);
	}
}
