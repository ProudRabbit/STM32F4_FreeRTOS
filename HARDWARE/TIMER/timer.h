#ifndef _TIMER_H_
#define _TIMER_H_
#include "sys.h"

//ͨ�ö�ʱ��3��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc);

//ͨ�ö�ʱ��2��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����������ݰ�����ʱ��
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);


#endif

