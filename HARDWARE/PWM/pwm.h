#ifndef _PWM_H_
#define _PWM_H_
#include "sys.h"


extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值 


//定时器TIM14 PWM输出
//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//输出脚PF9
void TIM14_PWM_Init(u32 arr,u32 psc);					//定时器TIM14 PWM输出

//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
//输入脚PA0
void TIM5_CH1_Cap_Init(u32 arr,u32 psc);		//定时器TIM5输入捕获


#endif


