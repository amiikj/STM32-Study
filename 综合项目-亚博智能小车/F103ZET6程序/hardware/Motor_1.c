#include "stm32f10x.h"

/**
 * @brief  初始化 M1 电机
 * @param  无
 */
void Motor1_Init(void)
{
    /* 1. 开启时钟 (TIM8 挂载在 APB2) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM8, ENABLE);
    
    /* 注意：PC6/PC7 默认就是 TIM8_CH1/CH2，不需要 AFIO 重映射 */

    /* 2. GPIO 初始化 —— 复用推挽输出 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PC6, PC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 3. TIM8 时基单元 (PSC=0, ARR=3600-1, 20kHz) */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 3600 - 1;  // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;      // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);

    /* 4. 通道1（PC6 = IN2）PWM 配置 */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);

    /* 5. 通道2（PC7 = IN1）PWM 配置 */
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);

    /* 6. 🚨 高级定时器 TIM8 必须开启 PWM 输出使能 */
    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    /* 7. 启动定时器 */
    TIM_Cmd(TIM8, ENABLE);
}

/**
 * @brief  设置 M1 电机速度
 * @param  Speed: -100 ~ 100，正数正转，负数反转，0停止
 */
void Motor1_SetSpeed(int16_t Speed)
{
    int16_t compare_value;
    
    if (Speed > 100)  Speed = 100;
    if (Speed < -100) Speed = -100;

    // 把 -100~100 的百分比，换算成 0~3600 的 CCR 值
    compare_value = (Speed > 0 ? Speed : -Speed) * 36; 

    if (Speed > 0) // 正转
    {
        // 根据 AT8236 真值表，正转是 IN1=1, IN2=0
        // 但如果是 PWM 调速，通常 IN1=PWM，IN2=0
        // 根据原理图，IN1 是 M1A(PC7/TIM8_CH2)，IN2 是 M1B(PC6/TIM8_CH1)
        TIM_SetCompare2(TIM8, compare_value); // 设置捕获比较器的值CCR PC7 (IN1) = PWM
        TIM_SetCompare1(TIM8, 0);             // PC6 (IN2) = 0
    }
    else if (Speed < 0) // 反转
    {
        TIM_SetCompare2(TIM8, 0);             // PC7 (IN1) = 0
        TIM_SetCompare1(TIM8, compare_value); // PC6 (IN2) = PWM
    }
    else // 停止
    {
        TIM_SetCompare2(TIM8, 0);
        TIM_SetCompare1(TIM8, 0);
    }
}
/**
 * @brief  初始化 M2 电机
 * @param  无
 */
void Motor2_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM8, ENABLE);
    

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // PC6, PC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 3600 - 1;  // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;      // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);


    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);


    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);    /* 6. 🚨 高级定时器 TIM8 必须开启 PWM 输出使能 */
    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    TIM_Cmd(TIM8, ENABLE);
}

/**
 * @brief  设置 M2 电机速度
 * @param  Speed: -100 ~ 100，正数正转，负数反转，0停止
 */
void Motor2_SetSpeed(int16_t Speed)
{
    int16_t compare_value;
    
    if (Speed > 100)  Speed = 100;
    if (Speed < -100) Speed = -100;

 
    compare_value = (Speed > 0 ? Speed : -Speed) * 36; 

    if (Speed > 0) // 正转
    {
        
        TIM_SetCompare4(TIM8, compare_value);
        TIM_SetCompare3(TIM8, 0);           
    }
    else if (Speed < 0) 
    {
        TIM_SetCompare4(TIM8, 0);            
        TIM_SetCompare3(TIM8, compare_value); 
    }
    else
    {
        TIM_SetCompare3(TIM8, 0);
        TIM_SetCompare4(TIM8, 0);
    }
}

/**
 * @brief  初始化 M3 电机
 * @param  无
 */
void Motor3_Init(void)
{
    /* 1. 开启时钟：注意必须加上 AFIO 时钟！ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 🚨 必须开，重映射的前提

    /* 2. 🚨 开启 TIM1 全重映射 */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);    // 🚨 最关键的一行！

    /* 3. GPIO 初始化 —— PE9 和 PE11 配置为复用推挽输出 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11; // PE9, PE11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* 4. TIM1 时基单元配置 (20kHz) */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 3600 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    /* 5. 通道1（PE9 = IN1）PWM 配置 */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* 6. 通道2（PE11 = IN2）PWM 配置 */
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    /* 7. 🚨 TIM1 是高级定时器，必须开启 PWM 输出使能！ */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* 8. 启动定时器 */
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief  设置 M3 电机速度
 * @param  Speed: -100 ~ 100，正数正转，负数反转，0停止
 */
void Motor3_SetSpeed(int16_t Speed)
{
    int16_t compare_value;
    
    if (Speed > 100)  Speed = 100;
    if (Speed < -100) Speed = -100;

 
    compare_value = (Speed > 0 ? Speed : -Speed) * 36; 

    if (Speed > 0) // 正转
    {
        
        TIM_SetCompare2(TIM1, compare_value);
        TIM_SetCompare1(TIM1, 0);           
    }
    else if (Speed < 0) 
    {
        TIM_SetCompare2(TIM1, 0);            
        TIM_SetCompare1(TIM1, compare_value); 
    }
    else
    {
        TIM_SetCompare1(TIM1, 0);
        TIM_SetCompare2(TIM1, 0);
    }
}

/**
 * @brief  初始化 M4 电机
 * @param  无
 */
void Motor4_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_TIM1, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 🚨 必须开，重映射的前提

    /* 2. 🚨 开启 TIM1 全重映射 */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);    // 🚨 最关键的一行！

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; // PC6, PC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 3600 - 1;  // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;      // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);


    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);


    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);    /* 6. 🚨 高级定时器 TIM8 必须开启 PWM 输出使能 */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief  设置 M4 电机速度
 * @param  Speed: -100 ~ 100，正数正转，负数反转，0停止
 */
void Motor4_SetSpeed(int16_t Speed)
{
    int16_t compare_value;
    
    if (Speed > 100)  Speed = 100;
    if (Speed < -100) Speed = -100;

 
    compare_value = (Speed > 0 ? Speed : -Speed) * 36; 

    if (Speed > 0) // 正转
    {
        
        TIM_SetCompare4(TIM1, compare_value);
        TIM_SetCompare3(TIM1, 0);           
    }
    else if (Speed < 0) 
    {
        TIM_SetCompare4(TIM1, 0);            
        TIM_SetCompare3(TIM1, compare_value); 
    }
    else
    {
        TIM_SetCompare4(TIM1, 0);
        TIM_SetCompare3(TIM1, 0);
    }
}

