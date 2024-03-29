#include "timer.h"
#include "led.h"


//通用定时器3初始化
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


//通用定时器3初始化
//arr：自动重装值。
//psc：时钟预分频数
//用来捕获电容按键的时间
void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
	TIM_ICInitTypeDef	TIM_ICInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);				//使能TIM2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				//使能PORTA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;										//初始化PA2为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//无上下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);												//初始化PA5
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);			//复用PA5为TIM2
	
	TIM_Base_Initstructure.TIM_Period = arr;				//设置重装载值
	TIM_Base_Initstructure.TIM_Prescaler = psc;					//设置预分频系数
	TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分割，滤波器作用
	TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_Base_Initstructure);						//初始化TIM2

	TIM_ICInitStructure.TIM_Channel =	TIM_Channel_1;							//选择输入端IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						//设置时钟分割:TDTS = Tck_tim
	TIM_ICInitStructure.TIM_ICFilter = 0x00;											//ICIF=0x00配置输入滤波器，不滤波
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);						//允许更新和捕获中断
	
	TIM_Cmd(TIM2,ENABLE);													//使能定时器2
	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}
