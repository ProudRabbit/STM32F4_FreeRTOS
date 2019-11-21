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
#include "adc.h"


int main(void)
{
	
		u16 adcx;
		float temp;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);    //初始化延时函数
		uart_init(115200); 	//串口初始化 
		LED_Init();				  //初始化LED端口  
		//BEEP_Init();        //初始化蜂鸣器端口
		//TIM3_Init(10000-1,8400-1);	//定时器时钟为84MHz，分频系数为8400所以计数频率为10KHz
		KEY_Init();					//初始化按键
		//EXTIX_Init();       //初始化外部中断输入
		//IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
		//	TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
		//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 
		TPAD_Init(8);				//初始化触摸按键,以84/4=21Mhz频率计数
		Adc_Init();
	
		while(1)
		{
			adcx=Get_Adc_AverageVal(ADC_Channel_4,20);//获取通道 4 的转换值， 20 次取平均
			
			temp=(float)adcx*(3.3/4096); //获取计算后的带小数的实际电压值，比如 3.1111
			

			printf("%d,%f\r\n",adcx,temp);
			LED0=!LED0; 
			delay_ms(250);
		}
}

