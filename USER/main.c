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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口  
	//BEEP_Init();        //初始化蜂鸣器端口
	//TIM3_Init(10000-1,8400-1);	//定时器时钟为84MHz，分频系数为8400所以计数频率为10KHz
	KEY_Init();					//初始化按键
	//EXTIX_Init();       //初始化外部中断输入
	//IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
	//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 
	TPAD_Init(8);				//初始化触摸按键,以84/4=21Mhz频率计数
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
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//设置 DAC 值PA4
		}
		else if(key==2)
		{
			if(dacval>200)
				dacval-=200;
			else 
				dacval=0;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//设置 DAC 值PA4
		}
		if(t==10||key==KEY1_PRES||key==WKUP_RPES)
		//WKUP/KEY1 按下了,或者定时时间到了
		{
			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//读取前面设置 DAC 的值PA4
			temp=(float)adcx*(3.3/4096); //得到 DAC 电压值
			printf("DAC寄存器读取到的电压值%d,%f\r\n",adcx,temp);
			
			adcx=Get_Adc_AverageVal(ADC_Channel_4,10); //得到 ADC 转换值PF6
			temp=(float)adcx*(3.3/4096); //得到 ADC 电压值
			printf("ADC读取到的电压值%d,%f\r\n",adcx,temp);
			
			LED0=!LED0;
			t=0;
		}
		delay_ms(100);
	}
}




