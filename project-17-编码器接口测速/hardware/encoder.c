#include "stm32f10x.h"                  // Device header


void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //什么叫和外部模块的默认电平一致？
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//TIM_InternalClockConfig(TIM3);  //为什么不用开启时钟源？
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;  //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler= 1-1;  //为什么PSC=0？
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//什么叫代表高低电平极性不反转？，这个代码在此处的作用：谁先谁后算正转”。
//	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//
	//TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//为什么不能用 | ?
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//什么叫代表高低电平极性不反转？，这个代码在此处的作用
	TIM_ICInit(TIM3,&TIM_ICInitStruct);

	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);  //1.此处函数的后两个参数配置的寄存器和上面TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	//的效果一样，因为后配置的会覆盖前面配置的，所以上面两个实际上可以删
	//2.该函数一定要在TIM_ICInit(TIM3,&TIM_ICInitStruct);下面，防止覆盖
	//3.怎么用该函数该极性？什么叫改后两个参数中的一个？
	TIM_Cmd(TIM3,ENABLE);
}


int16_t Encoder_Get(void)
{
	int16_t temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);   //函数有啥用？
	return  temp; //为什么不能用TIM_GetCapture();
}
