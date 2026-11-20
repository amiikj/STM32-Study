#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "myDMA.h"
#include "Delay.h"

uint8_t DataA[] = {0x01,0x02,0x03,0x04};
uint8_t DataB[] = { 0, 0, 0, 0};
//const uint8_t aa = 0x66;//表示只读，和Flash存储器一样，即变成常量，无法赋值
int main(void)
{
	OLED_Init();  
	
	MyDMA_Init((uint32_t)DataA,(uint32_t)DataB,4); //为什么不用加取地址符号,为什么直接就是这两个变量的地址？
	
	OLED_ShowString(1,1,"DataA:");
	OLED_ShowString(3,1,"DataB:");
	
	OLED_ShowHexNum(1,8,(uint32_t)DataA,8);  //1.为什么这里又要要强转为uint32_t
	OLED_ShowHexNum(3,8,(uint32_t)DataB,8);  //2.而显示DataA[0]不用强转类型
	
	
	
//	OLED_ShowHexNum(2,1,(uint32_t)&ADC1->DR,8);  //现象1.会发现是2000 0000 即，0x2000 0000是运行内存SRAM,所以可知  
						//现象2.会发现其地址为0800 0xxx,即0x0800 0xxx是Flash内存
	while (1) 
    {
		DataA[0] ++;
		DataA[1] ++;
		DataA[2] ++;
		DataA[3] ++;
		OLED_ShowHexNum(2,1,DataA[0],2);
		OLED_ShowHexNum(2,4,DataA[1],2);
		OLED_ShowHexNum(2,7,DataA[2],2);
		OLED_ShowHexNum(2,10,DataA[3],2);
		OLED_ShowHexNum(4,1,DataB[0],2);
		OLED_ShowHexNum(4,4,DataB[1],2);
		OLED_ShowHexNum(4,7,DataB[2],2);
		OLED_ShowHexNum(4,10,DataB[3],2);
		
		Delay_ms(1000);
		
		MyDMA_Transfer();
		OLED_ShowHexNum(2,1,DataA[0],2);
		OLED_ShowHexNum(2,4,DataA[1],2);
		OLED_ShowHexNum(2,7,DataA[2],2);
		OLED_ShowHexNum(2,10,DataA[3],2);
		OLED_ShowHexNum(4,1,DataB[0],2);
		OLED_ShowHexNum(4,4,DataB[1],2);
		OLED_ShowHexNum(4,7,DataB[2],2);
		OLED_ShowHexNum(4,10,DataB[3],2);
		
		Delay_ms(1000);
	}
}
