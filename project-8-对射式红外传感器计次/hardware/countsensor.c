#include "stm32f10x.h"                  // Device header


uint16_t Countsensor_Count;

void Init_Countsensor(void)
{
	/*记得用江协的PPT辅助理解*/
	/*第一步初始化四个原件的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	/*EXTI,NVIC的两个外设的时钟是不用开启的*/
	
	/*第二步GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	/*第三步AFIO外设中断引脚选择配置*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

	/*第四步配置EXTI*/
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE ;
	EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt ;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	/*第五步配置NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	/*分组方式整个芯片只能用一种，所以最好放在主函数中*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
}

void EXTI15_10_IRQHandler(void)  /*中断函数都是无参无返回值的*/ /*中断函数不需要在头文件中声明，因为其为自动调用*/
{
	if(EXTI_GetITStatus(EXTI_Line14)  == SET)
	{
	 Countsensor_Count ++;
	 EXTI_ClearITPendingBit(EXTI_Line14);
	
	
	};



}

uint16_t Countsensor_Get(void)
{

	return Countsensor_Count;

}
