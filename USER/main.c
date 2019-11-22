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
//要写入到 W25Q128 的字符串数组
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)



int main(void)
{
	
	
	u8 key, datatemp[SIZE];
	u16 i=0;
	u32 FLASH_SIZE;


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
	//MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)SendBuff,SEND_BUF_SIZE);	//配置DMA
	//AT24CXX_Init(); //IIC 初始化
	W25QXX_Init(); //W25QXX 初始化
	
	while(W25QXX_ReadID()!=W25Q128)//检测不到 W25Q128
	{
		printf("W25Q128 Check Failed!Please Check!\r\n");
		LED0=!LED0;//DS0 闪烁
		delay_ms(200);
	}
	printf("W25Q128 Ready!\r\n");
	
	while(1)
	{
			key=KEY_Scan(0);
			if(key==KEY1_PRES)//KEY1 按下,写入 W25Q128
			{
			
				printf("Start Write W25Q128....\r\n");
				W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);
				//从倒数第 100 个地址处开始,写入 SIZE 长度的数据
				printf("W25Q128 Write Finished!\r\n");//提示完成
			}
			if(key==KEY0_PRES)//KEY0 按下,读取字符串并显示
			{
				printf("Start Read W25Q128.... \r\n");
				W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);
				//从倒数第 100 个地址处开始,读出 SIZE 个字节
				printf("The Data Readed Is: %s.\r\n",datatemp);//提示传送完成
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




