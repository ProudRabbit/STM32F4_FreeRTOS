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


int main(void)
{
	
		long long temp=0;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);    //初始化延时函数
		uart_init(115200); 	//串口初始化 
		LED_Init();				  //初始化LED端口  
		BEEP_Init();        //初始化蜂鸣器端口
		//TIM3_Init(10000-1,8400-1);	//定时器时钟为84MHz，分频系数为8400所以计数频率为10KHz
		KEY_Init();					//初始化按键
		//EXTIX_Init();       //初始化外部中断输入
		//IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
		TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
		TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 
	
	
   	while(1)
		{
			delay_ms(10);
			TIM_SetCompare1(TIM14,TIM_GetCapture1(TIM14)+1); 
			if(TIM_GetCapture1(TIM14)==300)TIM_SetCompare1(TIM14,0);			 
			if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
			{
				temp=TIM5CH1_CAPTURE_STA&0X3F; 
				temp*=0XFFFFFFFF;		 		         //溢出时间总和
				temp+=TIM5CH1_CAPTURE_VAL;		   //得到总的高电平时间
				printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
				TIM5CH1_CAPTURE_STA=0;			     //开启下一次捕获
			}
		}
}

