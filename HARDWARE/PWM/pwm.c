#include "pwm.h"


//定时器TIM14 PWM输出
//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//输出脚PF9
void TIM14_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);				//使能TIM14时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				//使能PORTF时钟
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);			//复用PF9为TIM14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//初始化PF9为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;								//上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);												//初始化PF9
	
	TIM_TimeBaseStructure.TIM_Period = arr;						//设置重装载值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;				//设置预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分割，滤波器作用
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);						//初始化TIM14
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出比较使能
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_Low; 				//输出极性低
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM14,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);						//使能预装载寄存器
	TIM_ARRPreloadConfig(TIM14,ENABLE);											//ARPE使能
	
	TIM_Cmd(TIM14,ENABLE);
}


//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
//输入脚PA0
void TIM5_CH1_Cap_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
	TIM_ICInitTypeDef	TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);				//使能TIM5时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				//使能PORTA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;										//初始化PA7为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);												//初始化PA0
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);			//复用PA0为TIM5
	
	TIM_Base_Initstructure.TIM_Period = arr;				//设置重装载值
	TIM_Base_Initstructure.TIM_Prescaler = psc;					//设置预分频系数
	TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分割，滤波器作用
	TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_Base_Initstructure);						//初始化TIM5

	TIM_ICInitStructure.TIM_Channel =	TIM_Channel_1;							//选择输入端IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						//配置输入分频，不分频
	TIM_ICInitStructure.TIM_ICFilter = 0x00;											//ICIF=0x00配置输入滤波器，不滤波
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);						//允许更新和捕获中断
	
	TIM_Cmd(TIM5,ENABLE);													//使能定时器5
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 		    

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//获取当前的捕获值.
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器5
	 			TIM_SetCounter(TIM5,0);
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

