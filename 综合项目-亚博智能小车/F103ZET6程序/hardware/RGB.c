#include "stm32f10x.h"
#include "Delay.h" // 假设你的工程里有Delay.h

/**
 * @brief 初始化RGB灯引脚 (PE2, PE3, PE4, PE7, PG1, PG2)
 */
void RGB_Init(void)
{
    /* 1. 开启 GPIOE 和 GPIOG 的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE);

    /* 2. 配置 GPIO 为推挽输出 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 左侧 RGB 灯 (PE2, PE3, PE4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // 右侧 RGB 灯的绿色 (PE7)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // 右侧 RGB 灯的红色和蓝色 (PG1, PG2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* 3. 初始化时全部熄灭（拉低） */
    GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
    GPIO_ResetBits(GPIOG, GPIO_Pin_1 | GPIO_Pin_2);
}

/**
 * @brief 设置左侧 RGB 灯颜色
 * @param R 1亮 0灭
 * @param G 1亮 0灭
 * @param B 1亮 0灭
 */
void Left_RGB_Set(uint8_t R, uint8_t G, uint8_t B)
{
    if(R) GPIO_SetBits(GPIOE, GPIO_Pin_2); else GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    if(G) GPIO_SetBits(GPIOE, GPIO_Pin_3); else GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    if(B) GPIO_SetBits(GPIOE, GPIO_Pin_4); else GPIO_ResetBits(GPIOE, GPIO_Pin_4);
}

/**
 * @brief 设置右侧 RGB 灯颜色
 * @param R 1亮 0灭
 * @param G 1亮 0灭
 * @param B 1亮 0灭
 */
void Right_RGB_Set(uint8_t R, uint8_t G, uint8_t B)
{
    if(R) GPIO_SetBits(GPIOG, GPIO_Pin_1); else GPIO_ResetBits(GPIOG, GPIO_Pin_1);
    if(G) GPIO_SetBits(GPIOE, GPIO_Pin_7); else GPIO_ResetBits(GPIOE, GPIO_Pin_7);
    if(B) GPIO_SetBits(GPIOG, GPIO_Pin_2); else GPIO_ResetBits(GPIOG, GPIO_Pin_2);
}
