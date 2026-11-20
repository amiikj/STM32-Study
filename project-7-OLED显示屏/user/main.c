#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();
	while (1) 
    {
	OLED_ShowChar(1,1,'A');
	OLED_ShowString(2,3,"I Love Cat");
	}
}
