#include "timer.h"
#include "led.h"


//ͨ�ö�ʱ��3��ʼ��
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


//ͨ�ö�ʱ��3��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����������ݰ�����ʱ��
void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
	TIM_ICInitTypeDef	TIM_ICInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);				//ʹ��TIM2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;										//��ʼ��PA2Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);												//��ʼ��PA5
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);			//����PA5ΪTIM2
	
	TIM_Base_Initstructure.TIM_Period = arr;				//������װ��ֵ
	TIM_Base_Initstructure.TIM_Prescaler = psc;					//����Ԥ��Ƶϵ��
	TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷָ�˲�������
	TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_Base_Initstructure);						//��ʼ��TIM2

	TIM_ICInitStructure.TIM_Channel =	TIM_Channel_1;							//ѡ�������IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_ICInitStructure.TIM_ICFilter = 0x00;											//ICIF=0x00���������˲��������˲�
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);						//������ºͲ����ж�
	
	TIM_Cmd(TIM2,ENABLE);													//ʹ�ܶ�ʱ��2
	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}
