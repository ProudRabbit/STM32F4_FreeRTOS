#include "pwm.h"


//��ʱ��TIM14 PWM���
//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//�����PF9
void TIM14_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);				//ʹ��TIM14ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				//ʹ��PORTFʱ��
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);			//����PF9ΪTIM14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//��ʼ��PF9Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;								//����
	GPIO_Init(GPIOF,&GPIO_InitStructure);												//��ʼ��PF9
	
	TIM_TimeBaseStructure.TIM_Period = arr;						//������װ��ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;				//����Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷָ�˲�������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);						//��ʼ��TIM14
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM����ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //����Ƚ�ʹ��
	TIM_OCInitStructure.TIM_OCPolarity= TIM_OCPolarity_Low; 				//������Ե�
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM14,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);						//ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM14,ENABLE);											//ARPEʹ��
	
	TIM_Cmd(TIM14,ENABLE);
}


//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
//�����PA0
void TIM5_CH1_Cap_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_Base_Initstructure;
	TIM_ICInitTypeDef	TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);				//ʹ��TIM5ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);				//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;										//��ʼ��PA7Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);												//��ʼ��PA0
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);			//����PA0ΪTIM5
	
	TIM_Base_Initstructure.TIM_Period = arr;				//������װ��ֵ
	TIM_Base_Initstructure.TIM_Prescaler = psc;					//����Ԥ��Ƶϵ��
	TIM_Base_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷָ�˲�������
	TIM_Base_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_Base_Initstructure);						//��ʼ��TIM5

	TIM_ICInitStructure.TIM_Channel =	TIM_Channel_1;							//ѡ�������IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						//���������Ƶ������Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x00;											//ICIF=0x00���������˲��������˲�
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);						//�������ºͲ����ж�
	
	TIM_Cmd(TIM5,ENABLE);													//ʹ�ܶ�ʱ��5
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		    

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��5
	 			TIM_SetCounter(TIM5,0);
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}
