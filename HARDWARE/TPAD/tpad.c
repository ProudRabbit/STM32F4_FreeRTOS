#include "tpad.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"


vu16 tpad_default_val = 0;		//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��

//��ʼ����������
//��ÿ��ص�ʱ����������ȡֵ.
//psc:��Ƶϵ��,ԽС,������Խ��.
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
u8 TPAD_Init(u8 psc)
{
	u16 buf[10],temp;
	u8 i,j;
	
	TIM2_CH1_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);	//����TIM2�ķ�Ƶϵ��
	
	for(i = 0;i < 10;i++)
	{
		buf[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	
//��������
	for(i = 0;i < 9;i++)					
	{
		for(j = 0; j < 9-i;j++)
		{
			if(buf[j]>buf[j+1])
			{
				temp = buf[j+1];
				buf[j+1] = buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp = 0;
	for(i = 2;i < 8;i++)			//ȡ�м�6�����ݽ���ƽ��
		temp+=buf[i];
	tpad_default_val = temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val > (TPAD_ARR_MAX_VAL/2))					//��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!
		return 1;
	return 0;
	
}

//�õ���ʱ������ֵ
//�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
//����ֵ������ֵ/����ֵ����ʱ������·��أ�
u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2,TIM_IT_CC1)==RESET)		//�ȴ�����������
	{
		if(TIM_GetCounter(TIM2)>tpad_default_val-500)			//��ʱ��,ֱ�ӷ���CNT��ֵ
			return TIM_GetCounter(TIM2);
	}
	return TIM_GetCapture1(TIM2);
}


//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp = 0,res = 0;
	while(n--)
	{
		temp = TPAD_Get_Val();
		if(temp>res)
			res = temp;
	}
	return res;
}

//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;	//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;	//֧������	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))//����tpad_default_val+TPAD_GATE_VAL,��С��10��tpad_default_val,����Ч
	{							 
		if((keyen==0)&&(rval>(tpad_default_val+TPAD_GATE_VAL)))	//����tpad_default_val+TPAD_GATE_VAL,��Ч
		{
			res=1;
		}	   
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}

//��λһ��
//�ͷŵ��ݵ������������ʱ���ļ���ֵ
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;										//��ʼ��A5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;								//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;								//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);							//�ŵ�
	delay_ms(5);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1|TIM_IT_Update);
	TIM_SetCounter(TIM2,0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//��������
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

