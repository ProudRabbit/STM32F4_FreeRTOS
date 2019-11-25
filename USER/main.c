#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 256					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������

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





EventGroupHandle_t EventGroupHandler;	//�¼���־����

#define EVENTBIT_0	(1<<0)				//�¼�λ
#define EVENTBIT_1	(1<<1)
#define EVENTBIT_2	(1<<2)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2)






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

    //�����¼���־��
	EventGroupHandler=xEventGroupCreate();	 //�����¼���־��

	
	//���������¼�λ������
    xTaskCreate((TaskFunction_t )eventsetbit_task,             
                (const char*    )"eventsetbit_task",           
                (uint16_t       )EVENTSETBIT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTSETBIT_TASK_PRIO,        
                (TaskHandle_t*  )&EventSetBit_Handler);   	
    //�����¼���־�鴦������
    xTaskCreate((TaskFunction_t )eventgroup_task,             
                (const char*    )"eventgroup_task",           
                (uint16_t       )EVENTGROUP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTGROUP_TASK_PRIO,        
                (TaskHandle_t*  )&EventGroupTask_Handler);  
	//�����¼���־���ѯ����
    xTaskCreate((TaskFunction_t )eventquery_task,             
                (const char*    )"eventquery_task",           
                (uint16_t       )EVENTQUERY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTQUERY_TASK_PRIO,        
                (TaskHandle_t*  )&EventQueryTask_Handler);  



	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

//�����¼�λ������
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
					printf("�¼�0���\r\n");
					break;
				case KEY1_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
					printf("�¼�1���\r\n");
					break;
				case KEY2_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);
					printf("�¼�2���\r\n");
					break;	
			}
		}
        vTaskDelay(10); //��ʱ10ms��Ҳ����10��ʱ�ӽ���
	}
}

//�¼���־�鴦������
void eventgroup_task(void *pvParameters)
{
	EventBits_t EventValue;
	while(1)
	{

		if(EventGroupHandler!=NULL)
		{
			//�ȴ��¼����е���Ӧ�¼�λ
			EventValue=xEventGroupWaitBits((EventGroupHandle_t	)EventGroupHandler,		
										   (EventBits_t			)EVENTBIT_ALL,
										   (BaseType_t			)pdTRUE,				
										   (BaseType_t			)pdTRUE,
								           (TickType_t			)portMAX_DELAY);	
			printf("�¼���־���ֵ:%d\r\n",EventValue);
			LED1=!LED1;	
		}
		else
		{
			vTaskDelay(10); //��ʱ10ms��Ҳ����10��ʱ�ӽ���
		}
	}
}

//�¼���ѯ����
void eventquery_task(void *pvParameters)
{	
	u8 num=0;
	EventBits_t NewValue,LastValue;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			NewValue=xEventGroupGetBits(EventGroupHandler);	//��ȡ�¼����
			if(NewValue!=LastValue)
			{
				LastValue=NewValue;
				printf("�¼���־���ֵ:%d\r\n",NewValue);
			}
		}
		num++;
		if(num==10) 	//ÿ500msLED0��˸һ��
		{
			num=0;
			LED0=!LED0;	
		}
		vTaskDelay(50); //��ʱ50ms��Ҳ����50��ʱ�ӽ���
	}
}





