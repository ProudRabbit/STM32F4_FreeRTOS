#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
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


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数


int main(void)
{
	
	//LED_Init();				  //初始化LED端口
	//KEY_Init();					//初始化按键
	//TPAD_Init(8);				//初始化触摸按键,以84/4=21Mhz频率计数
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
	//W25QXX_Init(); //W25QXX 初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	LED_Init();		        //初始化LED端口
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
	
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=~LED0;
        vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(800);
    }
}



