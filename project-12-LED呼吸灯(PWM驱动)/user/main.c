#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "pwm.h"
#include "Delay.h"


uint16_t a;
int main(void)
{
	OLED_Init();
	PWM_Init();
	while (1) 
    {
		for (a =0 ;a<=100;a ++)
		{
		PWM_SetCompare1(a);
		Delay_ms(10);
		}
		
		for (a =100 ;a>=0;a --)
		{
		PWM_SetCompare1(a);
		Delay_ms(10);
		}
		
		
	}
}
