#ifndef _DAC_H_
#define _DAC_H_

#include "sys.h"

//DACͨ��һ(PA4)��ʼ��
void Dac1_Init(void);
//����DAC���ֵ(PA4)
//val��0~3300��Ӧ0~3.3V
void Dac1_Set_Val(u16 val);




#endif

