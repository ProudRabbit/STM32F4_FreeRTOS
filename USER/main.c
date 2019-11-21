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

int main(void)
{

	
	//long long temp=0;
	
	u16 adcx,t;
	float temp;
	u8 key;
	u16 dacval=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	//BEEP_Init();        //��ʼ���������˿�
	//TIM3_Init(10000-1,8400-1);	//��ʱ��ʱ��Ϊ84MHz����Ƶϵ��Ϊ8400���Լ���Ƶ��Ϊ10KHz
	KEY_Init();					//��ʼ������
	//EXTIX_Init();       //��ʼ���ⲿ�ж�����
	//IWDG_Init(4,500);			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
	//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz     
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� 
	TPAD_Init(8);				//��ʼ����������,��84/4=21MhzƵ�ʼ���
	Adc_Init();
	Dac1_Init();

	DAC_SetChannel1Data(DAC_Align_12b_R, dacval);
	while(1)
	{
		t++;
		key=KEY_Scan(0);
		if(key==WKUP_RPES)
		{
			if(dacval<4000)
				dacval+=200;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//���� DAC ֵPA4
		}
		else if(key==2)
		{
			if(dacval>200)
				dacval-=200;
			else 
				dacval=0;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//���� DAC ֵPA4
		}
		if(t==10||key==KEY1_PRES||key==WKUP_RPES)
		//WKUP/KEY1 ������,���߶�ʱʱ�䵽��
		{
			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//��ȡǰ������ DAC ��ֵPA4
			temp=(float)adcx*(3.3/4096); //�õ� DAC ��ѹֵ
			printf("DAC�Ĵ�����ȡ���ĵ�ѹֵ%d,%f\r\n",adcx,temp);
			
			adcx=Get_Adc_AverageVal(ADC_Channel_4,10); //�õ� ADC ת��ֵPF6
			temp=(float)adcx*(3.3/4096); //�õ� ADC ��ѹֵ
			printf("ADC��ȡ���ĵ�ѹֵ%d,%f\r\n",adcx,temp);
			
			LED0=!LED0;
			t=0;
		}
		delay_ms(100);
	}
}




