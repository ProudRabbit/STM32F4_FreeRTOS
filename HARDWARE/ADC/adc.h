#ifndef _ADC_H_
#define _ADC_H_

#include "sys.h"


//ADC��ʼ����ʹ�ù���ͨ��
//ʹ�õ���ADC3_CH4        PF6
void Adc_Init(void);
u16 Get_Adc_Val(u8 ch);
u16 Get_Adc_AverageVal(u8 ch,u8 times);

#endif

