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
#include "w25qxx.h"
//Ҫд�뵽 W25Q128 ���ַ�������
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)



int main(void)
{
	
	
	u8 key, datatemp[SIZE];
	u16 i=0;
	u32 FLASH_SIZE;


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
	//AT24CXX_Init(); //IIC ��ʼ��
	W25QXX_Init(); //W25QXX ��ʼ��
	
	while(W25QXX_ReadID()!=W25Q128)//��ⲻ�� W25Q128
	{
		printf("W25Q128 Check Failed!Please Check!\r\n");
		LED0=!LED0;//DS0 ��˸
		delay_ms(200);
	}
	printf("W25Q128 Ready!\r\n");
	
	while(1)
	{
			key=KEY_Scan(0);
			if(key==KEY1_PRES)//KEY1 ����,д�� W25Q128
			{
			
				printf("Start Write W25Q128....\r\n");
				W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);
				//�ӵ����� 100 ����ַ����ʼ,д�� SIZE ���ȵ�����
				printf("W25Q128 Write Finished!\r\n");//��ʾ���
			}
			if(key==KEY0_PRES)//KEY0 ����,��ȡ�ַ�������ʾ
			{
				printf("Start Read W25Q128.... \r\n");
				W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);
				//�ӵ����� 100 ����ַ����ʼ,���� SIZE ���ֽ�
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




