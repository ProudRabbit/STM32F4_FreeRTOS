#include "led.h"

//LED��ʼ��
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				//ʹ��AHB1ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;				//��ʼ��F9��F10Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;								//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;								//����
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
}

