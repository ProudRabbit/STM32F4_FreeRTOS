#ifndef _TPAD_H_
#define _TPAD_H_

#include "sys.h"


#define TPAD_ARR_MAX_VAL  0XFFFFFFFF     //最大ARR值（TIM2是32位的定时器）
#define TPAD_GATE_VAL 	100	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.


extern vu16 tpad_default_val;

u8 TPAD_Init(u8 psc);
u16 TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8 TPAD_Scan(u8 mod);
void TPAD_Reset(void);


#endif


