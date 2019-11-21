#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "iwdg.h"
#include "timer.h"
#include "pwm.h"
#include "tpad.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"

/*�������ݳ���,��õ��� sizeof(TEXT_TO_SEND)+2 ��������.*/
#define SEND_BUF_SIZE 8200
u8 SendBuff[SEND_BUF_SIZE]; //�������ݻ�����
const u8 TEXT_TO_SEND[]={"ALIENTEK Explorer STM32F4 DMA ����ʵ��"};


int main(void)
{
	/*
	u16 adcx,t;
	float temp;
	u8 key;
	u16 dacval=0;
	*/

	u16 i;
	u8 t=0,j,mask = 0;
	float pro=0;//����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�
	KEY_Init();					//��ʼ������
	TPAD_Init(8);				//��ʼ����������,��84/4=21MhzƵ�ʼ���
	//BEEP_Init();        //��ʼ���������˿�
	//TIM3_Init(10000-1,8400-1);	//��ʱ��ʱ��Ϊ84MHz����Ƶϵ��Ϊ8400���Լ���Ƶ��Ϊ10KHz
	//EXTIX_Init();       //��ʼ���ⲿ�ж�����
	//IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
	//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ���
	//Adc_Init();
	//Dac1_Init();

	/*DMA2,STEAM7,CH4,����Ϊ���� 1,�洢��Ϊ SendBuff,����Ϊ:SEND_BUF_SIZE.*/
	MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff,SEND_BUF_SIZE);	//����DMA

	j=sizeof(TEXT_TO_SEND);
	for(i=0;i<SEND_BUF_SIZE;i++)//��� ASCII �ַ�������
	{
		if(t>=j)//���뻻�з�
		{
			if(mask)
			{
				SendBuff[i]=0x0a;t=0;
			}
			else
			{
				SendBuff[i]=0x0d;mask++;
			}
		}
		else//���� TEXT_TO_SEND ���
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];t++;
		}
	}
	i = 0;
	while(1)
	{
		t=KEY_Scan(0);
		if(t==KEY0_PRES) //KEY0 ����
		{
			printf("\r\nDMA DATA:\r\n");

			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ��� 1 �� DMA ����
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); //��ʼһ�� DMA ���䣡
			while(1)
			{
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ� DMA2_Steam7 �������
				{
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//�崫����ɱ�־
					break;
				}
				pro=DMA_GetCurrDataCounter(DMA2_Stream7);//�õ���ǰʣ��������
				pro=1-pro/SEND_BUF_SIZE;//�õ��ٷֱ�
				pro*=100; //���� 100 ��
			}
			//printf("��ǰʣ���������:%f%%.\r\n",pro);
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������
			i=0;
		}
	}
}


