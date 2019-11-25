#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 256					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄


void Timer_task(void* pvParameters);			//任务函数
#define TIMER_STK_SIZE 50					//定义任务堆栈大小
#define TIMER_TASK_PRIO 2 					//定义任务优先级
TaskHandle_t TIMER_Task_Handler;				//定义任务句柄



TimerHandle_t OneShotTimer_Handle;	//单次定时器句柄
TimerHandle_t AutoReloadTimer_Handle;	//周期定时器句柄

void AutoReloadCallback(TimerHandle_t xTimer); 	//周期定时器回调函数
void OneShotCallback(TimerHandle_t xTimer);		//单次定时器回调函数




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

	AutoReloadTimer_Handle = xTimerCreate((const char *				) "AutoReloadTimer",
										  (TickType_t				) 1000,
										  (UBaseType_t				) pdTRUE,
										  (void * 					) 1,
 										  (TimerCallbackFunction_t	) AutoReloadCallback );
	
	OneShotTimer_Handle = xTimerCreate((const char *			) "OneShotTimer",
									   (TickType_t				) 2000,
									   (UBaseType_t				) pdFALSE,
									   (void * 					) 2,
 									   (TimerCallbackFunction_t	) OneShotCallback );
	
	//创建TIMER任务
	xTaskCreate((TaskFunction_t	)Timer_task,				//任务函数
				(char*			)"Timer_task",			//任务名称
				(uint16_t		)TIMER_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)TIMER_TASK_PRIO,			//任务的优先级
				(TaskHandle_t*	)&TIMER_Task_Handler);	//任务句柄


	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void Timer_task(void* pvParameters)
{
	u8 key,num;
	while (1)
	{
		if ((AutoReloadTimer_Handle!=NULL)&&(OneShotTimer_Handle!=NULL))
		{
			key = KEY_Scan(0);
			switch (key)
			{
				case WKUP_PRES:     //当key_up按下的话打开周期定时器
					xTimerStart(AutoReloadTimer_Handle,0);	//开启周期定时器
					printf("开启定时器1\r\n");
					break;
				case KEY0_PRES:		//当key0按下的话打开单次定时器
					xTimerStart(OneShotTimer_Handle,0);		//开启单次定时器
					printf("开启定时器2\r\n");
					break;
				case KEY1_PRES:		//当key1按下话就关闭定时器
					xTimerStop(AutoReloadTimer_Handle,0); 	//关闭周期定时器
					xTimerStop(OneShotTimer_Handle,0); 		//关闭单次定时器
					printf("关闭定时器1和2\r\n");
					break;	
			}	
		}
		num++;
		if (num == 50)
		{
			num = 0;
			LED0=!LED0;
		}
		vTaskDelay(10);
	}
}

//周期定时器的回调函数
void AutoReloadCallback(TimerHandle_t xTimer)
{
	static u8 tmr1_num=0;
	tmr1_num++;									//周期定时器执行次数加1
	printf("周期定时器1执行次数：%d\r\n",tmr1_num);
}

//单次定时器的回调函数
void OneShotCallback(TimerHandle_t xTimer)
{
	static u8 tmr2_num = 0;
	tmr2_num++;		//周期定时器执行次数加1
	printf("单次定时器2执行次数：%d\r\n",tmr2_num);
	LED1=!LED1;
    printf("定时器2运行结束\r\n");
}





