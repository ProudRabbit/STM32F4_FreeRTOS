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
#include "iic.h"
#include "24cxx.h"

const u8 TEXT_Buffer[]={"Explorer STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)


int main(void)
{
	
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];

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
	//MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff,SEND_BUF_SIZE);	//����DMA

	AT24CXX_Init(); //IIC ��ʼ��
	
	while(AT24CXX_Check())//��ⲻ�� 24c02
	{
		printf("24C02 Check Failed!Please Check!\r\n");
		LED0=!LED0;//DS0 ��˸
	}
	printf("24C02 Ready!\r\n");

	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1 ����,д�� 24C02
		{
		
			printf("Start Write 24C02....\r\n");
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			printf("24C02 Write Finished!\r\n");//��ʾ�������
		}
		if(key==KEY0_PRES)//KEY0 ����,��ȡ�ַ�������ʾ
		{
			printf("Start Read 24C02.... \r\n");
			AT24CXX_Read(0,datatemp,SIZE);
			printf("The Data Readed Is: %s.\r\n",datatemp);//��ʾ�������
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




