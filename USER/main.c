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


int main(void)
{
	
		u16 adcx;
		float temp;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);    //��ʼ����ʱ����
		uart_init(115200); 	//���ڳ�ʼ�� 
		LED_Init();				  //��ʼ��LED�˿�  
		//BEEP_Init();        //��ʼ���������˿�
		//TIM3_Init(10000-1,8400-1);	//��ʱ��ʱ��Ϊ84MHz����Ƶϵ��Ϊ8400���Լ���Ƶ��Ϊ10KHz
		KEY_Init();					//��ʼ������
		//EXTIX_Init();       //��ʼ���ⲿ�ж�����
		//IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
		//	TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz     
		//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� 
		TPAD_Init(8);				//��ʼ����������,��84/4=21MhzƵ�ʼ���
		Adc_Init();
	
		while(1)
		{
			adcx=Get_Adc_AverageVal(ADC_Channel_4,20);//��ȡͨ�� 4 ��ת��ֵ�� 20 ��ȡƽ��
			
			temp=(float)adcx*(3.3/4096); //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������ 3.1111
			

			printf("%d,%f\r\n",adcx,temp);
			LED0=!LED0; 
			delay_ms(250);
		}
}

