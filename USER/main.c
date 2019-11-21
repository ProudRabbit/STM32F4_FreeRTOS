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

/*发送数据长度,最好等于 sizeof(TEXT_TO_SEND)+2 的整数倍.*/
#define SEND_BUF_SIZE 8200
u8 SendBuff[SEND_BUF_SIZE]; //发送数据缓冲区
const u8 TEXT_TO_SEND[]={"ALIENTEK Explorer STM32F4 DMA 串口实验"};


int main(void)
{
	/*
	u16 adcx,t;
	float temp;
	u8 key;
	u16 dacval=0;
	*/

	u16 i;
	u8 t=0,j,mask = 0;
	float pro=0;//进度

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口
	KEY_Init();					//初始化按键
	TPAD_Init(8);				//初始化触摸按键,以84/4=21Mhz频率计数
	//BEEP_Init();        //初始化蜂鸣器端口
	//TIM3_Init(10000-1,8400-1);	//定时器时钟为84MHz，分频系数为8400所以计数频率为10KHz
	//EXTIX_Init();       //初始化外部中断输入
	//IWDG_Init(4,500);			//与分频数为64,重载值为500,溢出时间为1s	
	//TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数
	//Adc_Init();
	//Dac1_Init();

	/*DMA2,STEAM7,CH4,外设为串口 1,存储器为 SendBuff,长度为:SEND_BUF_SIZE.*/
	MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff,SEND_BUF_SIZE);	//配置DMA

	j=sizeof(TEXT_TO_SEND);
	for(i=0;i<SEND_BUF_SIZE;i++)//填充 ASCII 字符集数据
	{
		if(t>=j)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;t=0;
			}
			else
			{
				SendBuff[i]=0x0d;mask++;
			}
		}
		else//复制 TEXT_TO_SEND 语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];t++;
		}
	}
	i = 0;
	while(1)
	{
		t=KEY_Scan(0);
		if(t==KEY0_PRES) //KEY0 按下
		{
			printf("\r\nDMA DATA:\r\n");

			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口 1 的 DMA 发送
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); //开始一次 DMA 传输！
			while(1)
			{
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待 DMA2_Steam7 传输完成
				{
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清传输完成标志
					break;
				}
				pro=DMA_GetCurrDataCounter(DMA2_Stream7);//得到当前剩余数据数
				pro=1-pro/SEND_BUF_SIZE;//得到百分比
				pro*=100; //扩大 100 倍
			}
			//printf("当前剩余的数据量:%f%%.\r\n",pro);
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行
			i=0;
		}
	}
}


