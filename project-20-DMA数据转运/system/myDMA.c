#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)  //为什么DataA与DataB的要为uint32_t?
{
	MyDMA_Size = Size;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //为什么软件触发不能和自动重装一起使用？
	DMA_InitStructure.DMA_BufferSize = Size;  //这个参数是关于传输计数器的吗
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;  //存储器到存储器就是软件触发码？
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  //什么时候算是有多个通道？
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);  //为什么要关掉DMA
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE); 

	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);  //为什么是RESET
	DMA_ClearFlag(DMA1_FLAG_TC1);  //为什么要清除？
}
