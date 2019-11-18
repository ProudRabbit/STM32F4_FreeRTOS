#include "timer.h"
#include "led.h"


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
		TIM_Base_Initstructure.TIM_Period = psc;					//����Ԥ��Ƶϵ��
		TIM_Base_Initstructure.TIM_Prescaler = arr;				//������װ��ֵ
		TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷָ�˲�������
		TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3,&TIM_Base_Initstructure);						//��ʼ��TIM3

		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM3,ENABLE);
}

void	TIM3_IRQHandler()
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
		{
				LED1 = !LED1;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

