#include "stm32f10x.h"

volatile uint16_t g_speed_tick = 0; // 全局计时器，每1ms加1

/**
 * @brief  TIM2 初始化，1ms 中断一次
 */
void Timer_SpeedBase_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;  // ARR = 999
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC = 71，72MHz/72 = 1MHz，1us一步
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    /* 使能更新中断 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  TIM2 中断服务函数，每 1ms 进一次
 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  ///相当于判断是否过了1ms
    {
        g_speed_tick++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
