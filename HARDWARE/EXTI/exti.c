#include "exti.h"
#include "key.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "usart.h"

void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	KEY_Init();				//初始化按键对应的IO口为输入
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		//使能SYSCFG时钟,SYSCFG是挂载在APB2时钟下的，不是AHB2
	//RCC_AHB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);	//PE2连接线2	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);	//PE3连接线3	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);	//PE4连接线4	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);	//PA0连接线0

//配置EXTI_LINE0	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
//配置EXTI_LINE2,配置EXTI_LINE3,配置EXTI_LINE4	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel	= EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority	= 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
}


void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0)!=RESET)
	{
		delay_ms(10);	//消抖	注意使用delay_xms();delay_ms()会引起任务调度导致系统奔溃；
		if(WK_UP==1)
		{
			BEEP = !BEEP;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line2)!=RESET)
	{
		printf("外部中断2输出。。。。。\r\n");
		delay_xms(10);	//消抖	注意使用delay_xms();delay_ms()会引起任务调度导致系统奔溃；
		if(KEY2==0)	  
		{				 
			LED1=!LED1; 
		}	
	}		
	EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line3)!=RESET)
	{
		delay_ms(10);	//消抖	注意使用delay_xms();delay_ms()会引起任务调度导致系统奔溃；
		if(KEY1==0)
		{
			LED0 = !LED0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4)!=RESET)
	{
		delay_ms(10);	//消抖	注意使用delay_xms();delay_ms()会引起任务调度导致系统奔溃；
		if(KEY0==0)
		{
			LED0 = !LED0;
			LED1 = !LED1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

