#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 256					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄

void eventsetbit_task(void *pvParameters);
#define EVENTSETBIT_STK_SIZE 	256 
#define EVENTSETBIT_TASK_PRIO	2
TaskHandle_t EventSetBit_Handler;


void eventgroup_task(void *pvParameters);
#define EVENTGROUP_STK_SIZE 	256  
#define EVENTGROUP_TASK_PRIO	3
TaskHandle_t EventGroupTask_Handler;


void eventquery_task(void *pvParameters);
#define EVENTQUERY_STK_SIZE 	256
#define EVENTQUERY_TASK_PRIO	4
TaskHandle_t EventQueryTask_Handler;





EventGroupHandle_t EventGroupHandler;	//事件标志组句柄

#define EVENTBIT_0	(1<<0)				//事件位
#define EVENTBIT_1	(1<<1)
#define EVENTBIT_2	(1<<2)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2)






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

    //创建事件标志组
	EventGroupHandler=xEventGroupCreate();	 //创建事件标志组

	
	//创建设置事件位的任务
    xTaskCreate((TaskFunction_t )eventsetbit_task,             
                (const char*    )"eventsetbit_task",           
                (uint16_t       )EVENTSETBIT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTSETBIT_TASK_PRIO,        
                (TaskHandle_t*  )&EventSetBit_Handler);   	
    //创建事件标志组处理任务
    xTaskCreate((TaskFunction_t )eventgroup_task,             
                (const char*    )"eventgroup_task",           
                (uint16_t       )EVENTGROUP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTGROUP_TASK_PRIO,        
                (TaskHandle_t*  )&EventGroupTask_Handler);  
	//创建事件标志组查询任务
    xTaskCreate((TaskFunction_t )eventquery_task,             
                (const char*    )"eventquery_task",           
                (uint16_t       )EVENTQUERY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTQUERY_TASK_PRIO,        
                (TaskHandle_t*  )&EventQueryTask_Handler);  



	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

//设置事件位的任务
void eventsetbit_task(void *pvParameters)
{
	u8 key;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			key=KEY_Scan(0);
			switch(key)
			{
				case KEY0_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_0);
					printf("事件0完成\r\n");
					break;
				case KEY1_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
					printf("事件1完成\r\n");
					break;
				case KEY2_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);
					printf("事件2完成\r\n");
					break;	
			}
		}
        vTaskDelay(10); //延时10ms，也就是10个时钟节拍
	}
}

//事件标志组处理任务
void eventgroup_task(void *pvParameters)
{
	EventBits_t EventValue;
	while(1)
	{

		if(EventGroupHandler!=NULL)
		{
			//等待事件组中的相应事件位
			EventValue=xEventGroupWaitBits((EventGroupHandle_t	)EventGroupHandler,		
										   (EventBits_t			)EVENTBIT_ALL,
										   (BaseType_t			)pdTRUE,				
										   (BaseType_t			)pdTRUE,
								           (TickType_t			)portMAX_DELAY);	
			printf("事件标志组的值:%d\r\n",EventValue);
			LED1=!LED1;	
		}
		else
		{
			vTaskDelay(10); //延时10ms，也就是10个时钟节拍
		}
	}
}

//事件查询任务
void eventquery_task(void *pvParameters)
{	
	u8 num=0;
	EventBits_t NewValue,LastValue;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			NewValue=xEventGroupGetBits(EventGroupHandler);	//获取事件组的
			if(NewValue!=LastValue)
			{
				LastValue=NewValue;
				printf("事件标志组的值:%d\r\n",NewValue);
			}
		}
		num++;
		if(num==10) 	//每500msLED0闪烁一次
		{
			num=0;
			LED0=!LED0;	
		}
		vTaskDelay(50); //延时50ms，也就是50个时钟节拍
	}
}





