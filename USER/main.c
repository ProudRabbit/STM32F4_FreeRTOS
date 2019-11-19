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

int main(void)
{
	
	//long long temp=0;
		u8 t=0;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);    //��ʼ����ʱ����
		uart_init(115200); 	//���ڳ�ʼ�� 
		LED_Init();				  //��ʼ��LED�˿�  
		BEEP_Init();        //��ʼ���������˿�
		//TIM3_Init(10000-1,8400-1);	//��ʱ��ʱ��Ϊ84MHz����Ƶϵ��Ϊ8400���Լ���Ƶ��Ϊ10KHz
		KEY_Init();					//��ʼ������
		//EXTIX_Init();       //��ʼ���ⲿ�ж�����
		//IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
		//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz     
		//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� 
		TPAD_Init(8);
	
   	while(1)
		{
			if(TPAD_Scan(0))	//�ɹ�������һ��������(�˺���ִ��ʱ������15ms)
			{
				LED1=!LED1;		//LED1ȡ��
			}
			t++;
			if(t==15)		 
			{
				t=0;
				LED0=!LED0;		//LED0ȡ��,��ʾ������������
			}
			delay_ms(10);
		}
}

