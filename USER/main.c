#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 256					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄


void Interrupt_task(void* pvParameters);			//任务函数
#define Interrupt_STK_SIZE 256					//定义任务堆栈大小
#define Interrupt_TASK_PRIO 2 					//定义任务优先级
TaskHandle_t Interrupt_Task_Handler;				//定义任务句柄


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	uart_init(115200);		//初始化串口
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	TIM3_Init(10000-1, 8400-1);
	//创建开始任务
	xTaskCreate((TaskFunction_t	)start_task,			//任务函数
				(char*			)"start_task",			//任务名称
				(uint16_t		)START_STK_SIZE,		//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)START_TASK_PRIO,		//任务的优先级
				(TaskHandle_t*	)&StartTask_Handler);	//任务句柄

	vTaskStartScheduler();	//开启任务调度
}

void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();

	//创建LED0任务
	xTaskCreate((TaskFunction_t	)Interrupt_task,				//任务函数
				(char*			)"Interrupt_task",			//任务名称
				(uint16_t		)Interrupt_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)Interrupt_TASK_PRIO,			//任务的优先级
				(TaskHandle_t*	)&Interrupt_Task_Handler);	//任务句柄
							
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void Interrupt_task(void* pvParameters)
{
	static u32 total_num=0;
	
	while (1)
	{
		total_num++;
		if(total_num==5)
		{
			printf("关闭中断。。。。。。。\r\n");
			portDISABLE_INTERRUPTS();
			delay_xms(5000);
			portENABLE_INTERRUPTS();
			printf("打开中断。。。。。。。、\r\n");
		}

		LED0=~LED0;
        vTaskDelay(1000);
	}
}




