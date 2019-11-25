#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 256					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������


void KEY_task(void* pvParameters);			//������
#define KEY_STK_SIZE 50					//���������ջ��С
#define KEY_TASK_PRIO 2 					//�����������ȼ�
TaskHandle_t KEY_Task_Handler;				//����������

void USART_task(void* pvParameters);			//������
#define USART_STK_SIZE 200					//���������ջ��С
#define USART_TASK_PRIO 3 					//�����������ȼ�
TaskHandle_t USART_Task_Handler;				//����������


void config_task(void* pvParameters);			//������
#define CONFIG_STK_SIZE 50					//���������ջ��С
#define CONFEG_TASK_PRIO 4 					//�����������ȼ�
TaskHandle_t CONFIG_Task_Handler;				//����������

//������Ϣ���е�����
#define KEYMSG_Q_NUM 1		//������Ϣ���е�����
#define MESSAGE_Q_NUM 4		//�������ݵ���Ϣ���е�����
QueueHandle_t Key_Queue;	//����ֵ��Ϣ���о��
QueueHandle_t Message_Queue;	//��Ϣ���о��



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

	Key_Queue = xQueueCreate(KEYMSG_Q_NUM, sizeof(u8));
	Message_Queue = xQueueCreate(MESSAGE_Q_NUM, USART_REC_LEN);
	
	//����KEY����
	xTaskCreate((TaskFunction_t	)KEY_task,				//������
				(char*			)"KEY_task",			//��������
				(uint16_t		)KEY_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)KEY_TASK_PRIO,			//��������ȼ�
				(TaskHandle_t*	)&KEY_Task_Handler);	//������

					//������ʼ����
	xTaskCreate((TaskFunction_t	)USART_task,			//������
				(char*			)"USART_task",			//��������
				(uint16_t		)USART_STK_SIZE,		//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)USART_TASK_PRIO,		//��������ȼ�
				(TaskHandle_t*	)&USART_Task_Handler);	//������
					//������ʼ����
	xTaskCreate((TaskFunction_t	)config_task,			//������
				(char*			)"CONFEG_TASK_PRIO",	//��������
				(uint16_t		)CONFIG_STK_SIZE,		//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)CONFEG_TASK_PRIO,		//��������ȼ�
				(TaskHandle_t*	)&CONFIG_Task_Handler);	//������
							
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
		if(Key_Queue!=NULL && key)		//��Ϣ����Key_Queue�����ɹ�,���Ұ���������
		{
			err = xQueueSend(Key_Queue, &key, 10);	
			if (err == errQUEUE_FULL)
			{
				printf("����Key_Queue���������ݷ���ʧ��!\r\n");
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
			if(xQueueReceive(Message_Queue,buf,portMAX_DELAY)&&(USART_RX_STA&0x8000))		//������ϢMessage_Queue
			{

				USART_RX_STA=0;
				printf("�㷢�͵���ϢΪ��\r\n");
				for(t=0;t<USART_REC_LEN;t++)
				{
					USART_SendData(USART1, buf[t]);         //�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
			if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))	//������ϢKey_Queue
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


