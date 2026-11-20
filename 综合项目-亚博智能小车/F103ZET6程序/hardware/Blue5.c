#include "stm32f10x.h"                  // Device header
#include <stdio.h>

uint8_t Serial_RxFlag;
uint8_t Serial_RxData;

char Serial_RxPacket[100];///
uint8_t Serial_RxFlag;

char RxBuffer[50];   // 用于存放完整的指令字符串
uint8_t RxIndex = 0; // 用于记录存到了第几个字符

volatile uint32_t irq_test_count = 0; 

void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART5,&USART_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          
	NVIC_Init(&NVIC_InitStructure);        

	USART_Cmd(UART5,ENABLE);
}

void Serial_SendByte(uint16_t Byte)
{
	USART_SendData(UART5,Byte);
	while (USART_GetFlagStatus(UART5,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint16_t *Array,uint16_t Length)
{
	uint16_t i;
	for (i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}

}

void Serial_SendString(char *String) 
{
	uint16_t i;
	for(i=0;String[i]!=0;i++)
	{
		Serial_SendByte(String[i]);
	}

}

uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t Result = 1;
	while (y --)
	{
		Result *=x;
	
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10 + 0x30);
	}

}

int fputc(int ch,FILE *f)   
{
	Serial_SendByte(ch);
	return ch;


}	

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag==1)
	{
		Serial_RxFlag=0;
		return 1;
	
	}
	return 0;
}

uint8_t SerialGetData(void)
{
	return Serial_RxData;
}

void UART5_IRQHandler(void)
{
	 char res = USART_ReceiveData(UART5);
      irq_test_count++; // 👈 每进一次中断，加1
	
        // 简单的状态机：遇到 '$' 开始记录，遇到 '#' 结束
        if (res == '$') {
            RxIndex = 0; // 重新开始
            RxBuffer[RxIndex++] = res;
        } 
        else if (res == '#') {
            RxBuffer[RxIndex++] = res;
            RxBuffer[RxIndex] = '\0'; // 加上字符串结束符
            Serial_RxFlag = 1;        // 告诉主函数：一条完整指令收完了！
            // 注意：这里不要清空 RxIndex，等主函数处理完再清空或重置
        } 
        else if (RxIndex > 0 && RxIndex < 49) {
            RxBuffer[RxIndex++] = res; // 收到中间的逗号和数字
        }

        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
}

