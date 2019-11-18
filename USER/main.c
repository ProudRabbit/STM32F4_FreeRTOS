#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "iwdg.h"
#include "timer.h"

int main(void)
{
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);    //��ʼ����ʱ����
		uart_init(115200); 	//���ڳ�ʼ�� 
		LED_Init();				  //��ʼ��LED�˿�  
		BEEP_Init();        //��ʼ���������˿�
		TIM3_Init(10000-1,8400-1);	//��ʱ��ʱ��Ϊ84MHz����Ƶϵ��Ϊ8400���Լ���Ƶ��Ϊ10KHz
		KEY_Init();					//��ʼ������
		//EXTIX_Init();       //��ʼ���ⲿ�ж�����
		//IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	

		
		while(1)
		{
				delay_ms(10);
		}
}




