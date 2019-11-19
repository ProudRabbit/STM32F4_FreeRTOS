#include "tpad.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"


vu16 tpad_default_val = 0;		//空载的时候(没有手按下),计数器需要的时间

//初始化触摸按键
//获得空载的时候触摸按键的取值.
//psc:分频系数,越小,灵敏度越高.
//返回值:0,初始化成功;1,初始化失败
u8 TPAD_Init(u8 psc)
{
	u16 buf[10],temp;
	u8 i,j;
	
	TIM2_CH1_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);	//设置TIM2的分频系数
	
	for(i = 0;i < 10;i++)
	{
		buf[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	
//排序，升序
	for(i = 0;i < 9;i++)					
	{
		for(j = 0; j < 9-i;j++)
		{
			if(buf[j]>buf[j+1])
			{
				temp = buf[j+1];
				buf[j+1] = buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp = 0;
	for(i = 2;i < 8;i++)			//取中间6个数据进行平均
		temp+=buf[i];
	tpad_default_val = temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val > (TPAD_ARR_MAX_VAL/2))					//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
		return 1;
	return 0;
	
}

//得到定时器捕获值
//如果超时,则直接返回定时器的计数值.
//返回值：捕获值/计数值（超时的情况下返回）
u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2,TIM_IT_CC1)==RESET)		//等待捕获上升沿
	{
		if(TIM_GetCounter(TIM2)>tpad_default_val-500)			//超时了,直接返回CNT的值
			return TIM_GetCounter(TIM2);
	}
	return TIM_GetCapture1(TIM2);
}


//读取n次,取最大值
//n：连续获取的次数
//返回值：n次读数里面读到的最大读数值
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp = 0,res = 0;
	while(n--)
	{
		temp = TPAD_Get_Val();
		if(temp>res)
			res = temp;
	}
	return res;
}

//扫描触摸按键
//mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
//返回值:0,没有按下;1,有按下;
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,可以开始检测;>0,还不能开始检测	 
	u8 res=0;
	u8 sample=3;		//默认采样次数为3次	 
	u16 rval;
	if(mode)
	{
		sample=6;	//支持连按的时候，设置采样次数为6次
		keyen=0;	//支持连按	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))//大于tpad_default_val+TPAD_GATE_VAL,且小于10倍tpad_default_val,则有效
	{							 
		if((keyen==0)&&(rval>(tpad_default_val+TPAD_GATE_VAL)))	//大于tpad_default_val+TPAD_GATE_VAL,有效
		{
			res=1;
		}	   
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//至少要再过3次之后才能按键有效   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}

//复位一次
//释放电容电量，并清除定时器的计数值
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;										//初始化A5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;								//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);							//放电
	delay_ms(5);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1|TIM_IT_Update);
	TIM_SetCounter(TIM2,0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//无上下拉
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

