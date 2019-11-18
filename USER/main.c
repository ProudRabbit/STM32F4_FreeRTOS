#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	//LED0=0;					    //�ȵ������
	
	while(1)
	{
		printf("����һ���ⲿ�в���ʵ��\r\n");
		delay_ms(1000);
	}
}




