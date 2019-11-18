#include "timer.h"
#include "led.h"


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
		TIM_Base_Initstructure.TIM_Period = psc;					//设置预分频系数
		TIM_Base_Initstructure.TIM_Prescaler = arr;				//设置重装载值
		TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分割，滤波器作用
		TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
		TIM_TimeBaseInit(TIM3,&TIM_Base_Initstructure);						//初始化TIM3

		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM3,ENABLE);
}

void	TIM3_IRQHandler()
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
		{
				LED1 = !LED1;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

