#ifndef _TIMER_H_
#define _TIMER_H_
#include "sys.h"

//通用定时器3初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Init(u16 arr,u16 psc);

//通用定时器2初始化
//arr：自动重装值。
//psc：时钟预分频数
//用来捕获电容按键的时间
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);


#endif

