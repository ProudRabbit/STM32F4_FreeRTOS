#ifndef _DAC_H_
#define _DAC_H_

#include "sys.h"

//DAC通道一(PA4)初始化
void Dac1_Init(void);
//设置DAC输出值(PA4)
//val：0~3300对应0~3.3V
void Dac1_Set_Val(u16 val);




#endif

