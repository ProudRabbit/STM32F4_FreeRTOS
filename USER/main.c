#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 128					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄


void LED0_task(void* pvParameters);			//任务函数
#define LED0_STK_SIZE 50					//定义任务堆栈大小
#define LED0_TASK_PRIO 2 					//定义任务优先级
TaskHandle_t LED0_Task_Handler;				//定义任务句柄

void LED1_task(void* pvParameters);			//任务函数
#define LED1_STK_SIZE 50					//定义任务堆栈大小
#define LED1_TASK_PRIO 3 					//定义任务优先级
TaskHandle_t LED1_Task_Handler;				//定义任务句柄


void KEY_task(void* pvParameters);			//任务函数
#define KEY_STK_SIZE 100					//定义任务堆栈大小
#define KEY_TASK_PRIO 4 					//定义任务优先级
TaskHandle_t KEY_Task_Handler;				//定义任务句柄


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	uart_init(115200);		//初始化串口
	LED_Init();
	KEY_Init();

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
	xTaskCreate((TaskFunction_t	)LED0_task,				//任务函数
				(char*			)"LED0_task",			//任务名称
				(uint16_t		)LED0_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)LED0_TASK_PRIO,			//任务的优先级
				(TaskHandle_t*	)&LED0_Task_Handler);	//任务句柄
				
	//创建LED1任务
	xTaskCreate((TaskFunction_t	)LED1_task,				//任务函数
				(char*			)"LED1_task",			//任务名称
				(uint16_t		)LED1_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)LED1_TASK_PRIO,			//任务的优先级
				(TaskHandle_t*	)&LED1_Task_Handler);	//任务句柄


	//创建按键任务
	xTaskCreate((TaskFunction_t )KEY_task,				//任务函数
				(char*			)"KEY_task",			//任务名称
				(uint16_t		)KEY_STK_SIZE,			//任务堆栈大小
				(void*			)NULL,					//传递给任务的参数
				(UBaseType_t	)KEY_TASK_PRIO, 		//任务的优先级
				(TaskHandle_t*	)&KEY_Task_Handler);	//任务句柄


				
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void LED0_task(void * pvParameters)
{
	u16 i= 0;
	while(1)
	{
		i++;
		LED0 = 1;
		vTaskDelay(500);
		LED0 = 0;
		vTaskDelay(500);
		printf("led0_task运行了%d次\r\n",i);
	}
}


void LED1_task(void * pvParameters)
{
	u16 i = 0;
	while(1)
	{
		i++;
		LED1= 1;
		vTaskDelay(800);
		LED1 = 0;
		vTaskDelay(200);
		if(i > 5)
			vTaskDelete(NULL);
		printf("led1_task运行了%d次\r\n",i);
	}
}


void KEY_task(void * pvParameters)
{
	u8 key;
	while(1)
	{

		key = KEY_Scan(0);
		if(key == KEY0_PRES)
			vTaskSuspend(LED0_Task_Handler);
		else if(key == KEY1_PRES)
			vTaskResume(LED0_Task_Handler);
		vTaskDelay(20);
	}
}

