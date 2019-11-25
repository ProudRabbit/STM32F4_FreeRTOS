#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 256					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄


void KEY_task(void* pvParameters);			//任务函数
#define KEY_STK_SIZE 50					//定义任务堆栈大小
#define KEY_TASK_PRIO 2 					//定义任务优先级
TaskHandle_t KEY_Task_Handler;				//定义任务句柄

void USART_task(void* pvParameters);			//任务函数
#define USART_STK_SIZE 200					//定义任务堆栈大小
#define USART_TASK_PRIO 3 					//定义任务优先级
TaskHandle_t USART_Task_Handler;				//定义任务句柄


void config_task(void* pvParameters);			//任务函数
#define CONFIG_STK_SIZE 50					//定义任务堆栈大小
#define CONFEG_TASK_PRIO 4 					//定义任务优先级
TaskHandle_t CONFIG_Task_Handler;				//定义任务句柄

//按键消息队列的数量
#define KEYMSG_Q_NUM 1		//按键消息队列的数量
#define MESSAGE_Q_NUM 4		//发送数据的消息队列的数量
QueueHandle_t Key_Queue;	//按键值消息队列句柄
QueueHandle_t Message_Queue;	//信息队列句柄



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

	Key_Queue = xQueueCreate(KEYMSG_Q_NUM, sizeof(u8));
	Message_Queue = xQueueCreate(MESSAGE_Q_NUM, USART_REC_LEN);
	
	//创建KEY任务
	xTaskCreate((TaskFunction_t	)KEY_task,				//任务函数
				(char*			)"KEY_task",			//任务名称
				(uint16_t		)KEY_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)KEY_TASK_PRIO,			//任务的优先级
				(TaskHandle_t*	)&KEY_Task_Handler);	//任务句柄

					//创建开始任务
	xTaskCreate((TaskFunction_t	)USART_task,			//任务函数
				(char*			)"USART_task",			//任务名称
				(uint16_t		)USART_STK_SIZE,		//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)USART_TASK_PRIO,		//任务的优先级
				(TaskHandle_t*	)&USART_Task_Handler);	//任务句柄
					//创建开始任务
	xTaskCreate((TaskFunction_t	)config_task,			//任务函数
				(char*			)"CONFEG_TASK_PRIO",	//任务名称
				(uint16_t		)CONFIG_STK_SIZE,		//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)CONFEG_TASK_PRIO,		//任务的优先级
				(TaskHandle_t*	)&CONFIG_Task_Handler);	//任务句柄
							
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void KEY_task(void* pvParameters)
{
	u8 key;
    BaseType_t err;
	while(1)
	{
		key = KEY_Scan(1);
		if(Key_Queue!=NULL && key)		//消息队列Key_Queue创建成功,并且按键被按下
		{
			err = xQueueSend(Key_Queue, &key, 10);	
			if (err == errQUEUE_FULL)
			{
				printf("队列Key_Queue已满，数据发送失败!\r\n");
			}
		}
		vTaskDelay(20);
	}
}


void USART_task(void* pvParameters)
{
	u8 buf[USART_REC_LEN],t;
	while(1)
	{
		if(Message_Queue!= NULL)
		{
			if(xQueueReceive(Message_Queue,buf,portMAX_DELAY)&&(USART_RX_STA&0x8000))		//请求消息Message_Queue
			{

				USART_RX_STA=0;
				printf("你发送的信息为：\r\n");
				for(t=0;t<USART_REC_LEN;t++)
				{
					USART_SendData(USART1, buf[t]);         //向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
				printf("\r\n");
			}
		}
		delay_xms(20);
	}
}


void config_task(void* pvParameters)
{
	u8 key;
	while(1)
	{
		if(Key_Queue != NULL)
		{
			if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))	//请求消息Key_Queue
			{
				switch (key)
					{
						case WKUP_PRES:
							LED0 = !LED0;
							break;
						case KEY2_PRES:
							LED1 = !LED1;
							break;
					}
			}
		}
		vTaskDelay(10);
	}
}


