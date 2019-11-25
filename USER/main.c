#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 256					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������


void Timer_task(void* pvParameters);			//������
#define TIMER_STK_SIZE 50					//���������ջ��С
#define TIMER_TASK_PRIO 2 					//�����������ȼ�
TaskHandle_t TIMER_Task_Handler;				//����������



TimerHandle_t OneShotTimer_Handle;	//���ζ�ʱ�����
TimerHandle_t AutoReloadTimer_Handle;	//���ڶ�ʱ�����

void AutoReloadCallback(TimerHandle_t xTimer); 	//���ڶ�ʱ���ص�����
void OneShotCallback(TimerHandle_t xTimer);		//���ζ�ʱ���ص�����




int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	uart_init(115200);		//��ʼ������
	LED_Init();
	KEY_Init();
	
	//������ʼ����
	xTaskCreate((TaskFunction_t	)start_task,			//������
				(char*			)"start_task",			//��������
				(uint16_t		)START_STK_SIZE,		//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)START_TASK_PRIO,		//��������ȼ�
				(TaskHandle_t*	)&StartTask_Handler);	//������

	vTaskStartScheduler();	//�����������
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
	
	//����TIMER����
	xTaskCreate((TaskFunction_t	)Timer_task,				//������
				(char*			)"Timer_task",			//��������
				(uint16_t		)TIMER_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)TIMER_TASK_PRIO,			//��������ȼ�
				(TaskHandle_t*	)&TIMER_Task_Handler);	//������


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
				case WKUP_PRES:     //��key_up���µĻ������ڶ�ʱ��
					xTimerStart(AutoReloadTimer_Handle,0);	//�������ڶ�ʱ��
					printf("������ʱ��1\r\n");
					break;
				case KEY0_PRES:		//��key0���µĻ��򿪵��ζ�ʱ��
					xTimerStart(OneShotTimer_Handle,0);		//�������ζ�ʱ��
					printf("������ʱ��2\r\n");
					break;
				case KEY1_PRES:		//��key1���»��͹رն�ʱ��
					xTimerStop(AutoReloadTimer_Handle,0); 	//�ر����ڶ�ʱ��
					xTimerStop(OneShotTimer_Handle,0); 		//�رյ��ζ�ʱ��
					printf("�رն�ʱ��1��2\r\n");
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

//���ڶ�ʱ���Ļص�����
void AutoReloadCallback(TimerHandle_t xTimer)
{
	static u8 tmr1_num=0;
	tmr1_num++;									//���ڶ�ʱ��ִ�д�����1
	printf("���ڶ�ʱ��1ִ�д�����%d\r\n",tmr1_num);
}

//���ζ�ʱ���Ļص�����
void OneShotCallback(TimerHandle_t xTimer)
{
	static u8 tmr2_num = 0;
	tmr2_num++;		//���ڶ�ʱ��ִ�д�����1
	printf("���ζ�ʱ��2ִ�д�����%d\r\n",tmr2_num);
	LED1=!LED1;
    printf("��ʱ��2���н���\r\n");
}





