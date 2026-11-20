#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "servo.h"
#include "Delay.h"
#include "key.h"
#include "motor.h"
int16_t Speed =0;

uint16_t KeyNum;

int main(void)
{
	OLED_Init();
	Servo_Init();
	Key_Init();
	Motor_Init();
	
	
	Speed = 0;
	
	OLED_ShowString(1,1,"Speed:");
	while (1) 
    {
		 KeyNum=Key_GetNum();
		if (KeyNum == 1)
		{
			Speed +=10;
			if(Speed >100)
			{
			Speed = 0;
			}
		}
		
		if (KeyNum == 2)
		{
			Speed -=10;
			if(Speed < -100)
			{
				Speed = 0;
			}
		}
		
		Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(1,7,Speed,4);
	}
}
