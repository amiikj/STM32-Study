#include "stm32f10x.h"
#include "Encoder.h"

/**
 * @brief  初始化 H1 编码器 (TIM4_CH1=PD12, TIM4_CH2=PD13)
 */
void Encoder_H4_Init(void)
{
    /* 1. 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 🚨 新增！

    /* 2. 🚨 关键：开启 TIM4 重映射，把 CH1/CH2 搬到 PD12/PD13 */
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);          // 🚨 新增！
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   // 🚨 禁用 JTAG，保留 SWD
    /* 2. GPIO 初始化：PD12, PD13 上拉输入 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 3. TIM4 时基单元：PSC=0, ARR=65535 (16位最大值，防止溢出) */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;      // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    /* 4. 🚨 关键：开启 TIM4 编码器接口模式（TI1 和 TI2 双边沿，4倍频） */
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
    TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    /* 5. 清除计数器，启动定时器 */
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief  读取并清空编码器计数（增量）
 * @return 带符号的增量值（正数正转，负数反转）
 */
int16_t Encoder_H4_GetDelta(void)
{
    int16_t temp = (int16_t)TIM_GetCounter(TIM3); // 强制转换成有符号数,CNT记录电机触发了多少个信号
    TIM_SetCounter(TIM3, 0);                       // 读走清零
    return temp;
}
