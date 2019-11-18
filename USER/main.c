#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "iwdg.h"

int main(void)
{
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);    //��ʼ����ʱ����
		uart_init(115200); 	//���ڳ�ʼ�� 
		LED_Init();				  //��ʼ��LED�˿�  
		BEEP_Init();        //��ʼ���������˿�
		delay_ms(200);
		KEY_Init();					//��ʼ������
		//EXTIX_Init();       //��ʼ���ⲿ�ж�����
		IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
		LED0=0;					    //�ȵ������
		
		while(1)
		{
				if(KEY_Scan(0)==WKUP_RPES)
				{
					IWDG_Feed();
				}
				delay_ms(10);
		}
}




