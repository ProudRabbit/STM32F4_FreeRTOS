#ifndef _PWM_H_
#define _PWM_H_
#include "sys.h"


extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 


//��ʱ��TIM14 PWM���
//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//�����PF9
void TIM14_PWM_Init(u32 arr,u32 psc);					//��ʱ��TIM14 PWM���

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
//�����PA0
void TIM5_CH1_Cap_Init(u32 arr,u32 psc);		//��ʱ��TIM5���벶��


#endif


