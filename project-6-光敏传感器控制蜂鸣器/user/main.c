#include "stm32f10x.h"   // Device header
#include "light-sensor.h"
#include "loud.h"

int main(void)
{
	loud_Init();
	light_Init();
	
	while (1) 
    {
		if (GetNum() == 1 )
		{
			loud_On();
		}
		else
		{
			loud_Off();
		}
    }
}
