#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "iwdg.h"
#include "timer.h"
#include "pwm.h"
#include "tpad.h"

int main(void)
{
	
	//long long temp=0;
		u8 t=0;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);    //初始化延时函数
		uart_init(115200); 	//串口初始化 
		LED_Init();				  //初始化LED端口  
		BEEP_Init();        //初始化蜂鸣器端口
		//TIM3_Init(10000-1,8400-1);	//定时器时钟为84MHz，分频系数为8400所以计数频率为10KHz
		KEY_Init();					//初始化按键
		//EXTIX_Init();       //初始化外部中断输入
		//IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
		//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
		//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 
		TPAD_Init(8);
	
   	while(1)
		{
			if(TPAD_Scan(0))	//成功捕获到了一次上升沿(此函数执行时间至少15ms)
			{
				LED1=!LED1;		//LED1取反
			}
			t++;
			if(t==15)		 
			{
				t=0;
				LED0=!LED0;		//LED0取反,提示程序正在运行
			}
			delay_ms(10);
		}
}

