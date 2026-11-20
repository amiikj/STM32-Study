#include "stm32f10x.h"
#include "Delay.h"
#include "Motor_1.h"
#include "Encoder.h"
#include "Timer.h"
#include "OLED.h"
#include "RGB.h"
#include "Blue5.h"
#include <string.h>

extern volatile uint16_t g_speed_tick; // 引用 Timer.c 里的全局计时器
extern volatile uint32_t irq_count;  /////
extern volatile uint32_t irq_test_count;
extern char RxBuffer[50];   // 用于存放完整的指令字符串

uint8_t Statement = 0;

int16_t Speed_Delta=0;
int32_t total_delta = 0;   // 累加器
int8_t sample_count = 0;  // 计数次数
uint16_t last_tick = 0;
uint8_t Motor_Error_Num =0;
uint16_t Sign;  //红外信号
char RxData;

uint8_t  recover_state = 0; // 0:正常, 1:后退中, 2:转圈中
uint16_t recover_tick = 0;  // 记录动作开始的时间戳

int main(void)
{
	OLED_Init();
	Encoder_H4_Init();
	Timer_SpeedBase_Init();
	RGB_Init();
	Serial_Init();
	Motor1_Init();
	Motor2_Init();
	Motor3_Init();
	Motor4_Init();
    
///	Motor1_SetSpeed(70);
///	Motor2_SetSpeed(70);
/// Motor3_SetSpeed(-70);
///	Motor4_SetSpeed(-70);
	
	while(1) 
	{	
/*  	Sign = InfraredRemote_GetCommand();
		if (Sign==0x80)
		{
			Motor1_SetSpeed(70);
			Motor2_SetSpeed(70);
			Motor3_SetSpeed(-70);
			Motor4_SetSpeed(-70);
		}
		if (Sign==0x00)
		{
			Motor1_SetSpeed(0);
			Motor2_SetSpeed(0);
			Motor3_SetSpeed(0);
			Motor4_SetSpeed(0);
		}*/
		OLED_ShowString(1,1,"Speed_Delta:");
		OLED_ShowNum(2,1,Statement,4);
		OLED_ShowString(3, 1,(char*)RxBuffer);   // 显示中断次数
		if (Statement == 0)
		{
				if (Serial_GetRxFlag()==1)
				{
					RxData = SerialGetData();
					if (strstr(RxBuffer, "$1,0,0,0,0,0,0,0,0#") != NULL)
					{
						Motor1_SetSpeed(70);
						Motor2_SetSpeed(70);
						Motor3_SetSpeed(-70);
						Motor4_SetSpeed(-70);
					}
					
					if (strstr(RxBuffer, "$2,0,0,0,0,0,0,0,0#") != NULL)
					{
						Motor1_SetSpeed(-70);
						Motor2_SetSpeed(-70);
						Motor3_SetSpeed(70);
						Motor4_SetSpeed(70);
					}
					
					if (strstr(RxBuffer, "$3,0,0,0,0,0,0,0,0#") != NULL)
					{
						Motor1_SetSpeed(-70);
						Motor2_SetSpeed(-70);
						Motor3_SetSpeed(-70);
						Motor4_SetSpeed(-70);
					}
					
					if (strstr(RxBuffer, "$4,0,0,0,0,0,0,0,0#") != NULL)
					{
						Motor1_SetSpeed(70);
						Motor2_SetSpeed(70);
						Motor3_SetSpeed(70);
						Motor4_SetSpeed(70);
					}
					
					if (strstr(RxBuffer, "$0,0,0,0,0,0,0,0,0#") != NULL)
					{
						Motor1_SetSpeed(0);
						Motor2_SetSpeed(0);
						Motor3_SetSpeed(0);
						Motor4_SetSpeed(0);
					}
					
					if (strstr(RxBuffer, "$0,0,0,0,0,0,0,1,0#") != NULL)
					{
						Statement =1;
						total_delta = 0;
						sample_count = 0;
						Motor1_SetSpeed(-70);
						Motor2_SetSpeed(-70);
						Motor3_SetSpeed(70);
						Motor4_SetSpeed(70);
					}
					memset(RxBuffer, 0, sizeof(RxBuffer)); // 把数组里的字符全变成 0 (\0)                                     // 把指针归零，下次从头开始存
				}
			}	
		
		// ==========================================
        // 状态 1：自动堵转检测模式
        // ==========================================
		else if (Statement == 1)
		{
			// 1. 接收串口指令（随时可以按停止键退出）
			if (Serial_GetRxFlag()==1)
			{
				RxData = SerialGetData();
				if (strstr(RxBuffer, "$0,0,0,0,0,0,0,1,0#") != NULL)
				{
					Statement = 0;
					recover_state = 0; // 退出时清理状态
					Motor1_SetSpeed(0); Motor2_SetSpeed(0);
					Motor3_SetSpeed(0); Motor4_SetSpeed(0);
				}
				memset(RxBuffer, 0, sizeof(RxBuffer)); 
			}
			
			// 显示当前脉冲数，OLED 永远不会卡死了
			OLED_ShowNum(4, 1, total_delta, 6); 
			OLED_ShowNum(5, 1,recover_state , 6); 
			// ==========================================
			// 2. 处理堵转后的恢复动作（非阻塞状态机）
			// ==========================================
			if (recover_state != 0) 
			{
				// 状态 1：后退 800ms (通过 g_speed_tick 判断时间)
				if (recover_state == 1) 
				{
					if (g_speed_tick - recover_tick >= 800) 
					{
						recover_state = 2;           // 切换为转圈状态
						recover_tick = g_speed_tick; // 重新记录时间
						Motor1_SetSpeed(70); Motor2_SetSpeed(70);
						Motor3_SetSpeed(70); Motor4_SetSpeed(70);
					}
				}
				// 状态 2：转圈 1000ms
				else if (recover_state == 2) 
				{
					if (g_speed_tick - recover_tick >= 1000) 
					{
						recover_state = 0; // 恢复完毕
						Left_RGB_Set(0,0,0); Right_RGB_Set(0,0,0);
						Motor1_SetSpeed(70); Motor2_SetSpeed(70);
						Motor3_SetSpeed(70); Motor4_SetSpeed(70);
						
						// 清空计数器，并给 500ms 的起步缓冲 (sample_count 设为 -10)
						Encoder_H4_GetDelta(); 
						last_tick = g_speed_tick; 
						total_delta = 0;       
						sample_count = -10; 
					}
				}
			}
			// ==========================================
			// 3. 正常检测堵转
			// ==========================================
			else 
			{
				if (g_speed_tick - last_tick >= 50)
				{
					last_tick = g_speed_tick;
					Speed_Delta = Encoder_H4_GetDelta();
			
					total_delta += Speed_Delta;   
					sample_count++;
			
					if (sample_count >= 5)   // 攒够 250ms
					{
						if (total_delta < 20 && total_delta > -20) // 判定堵转 (阈值改成了20)
						{
							Left_RGB_Set(1,0,0); Right_RGB_Set(1,0,0);
							
							// 🚨 不再死等！立刻设置状态，退出这个判断
							recover_state = 1; 
							recover_tick = g_speed_tick;
							
							// 立即执行第一步：后退
							Motor1_SetSpeed(70); Motor2_SetSpeed(70);
							Motor3_SetSpeed(70); Motor4_SetSpeed(70);
						}
						else
						{
							total_delta = 0;   
							sample_count = 0;
						}
					}
				}
			}
		}
	}
     
}
