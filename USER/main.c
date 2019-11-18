#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "iwdg.h"

int main(void)
{
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);    //初始化延时函数
		uart_init(115200); 	//串口初始化 
		LED_Init();				  //初始化LED端口  
		BEEP_Init();        //初始化蜂鸣器端口
		delay_ms(200);
		KEY_Init();					//初始化按键
		//EXTIX_Init();       //初始化外部中断输入
		IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
		LED0=0;					    //先点亮红灯
		
		while(1)
		{
				if(KEY_Scan(0)==WKUP_RPES)
				{
					IWDG_Feed();
				}
				delay_ms(10);
		}
}




