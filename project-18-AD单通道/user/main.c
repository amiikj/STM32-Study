#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ad.h"

uint16_t AD_Value;
float Voltage;
int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1,1,"AD_Value:");
	OLED_ShowString(2,1,"Voltage:000");
	while (1) 
    {
		AD_Value = AD_GetValue();
		Voltage = (float)AD_Value / 4095 *3.3; //为什么？
		OLED_ShowNum(2,10,Voltage,1);
		OLED_ShowNum(2,11,(uint16_t)(Voltage*100)%100,2);//为什么要转换数据类型
		OLED_ShowNum(1,10,AD_Value,4); 
	}
}

//为什么取不到3.3？