#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 128					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������


void LED0_task(void* pvParameters);			//������
#define LED0_STK_SIZE 50					//���������ջ��С
#define LED0_TASK_PRIO 2 					//�����������ȼ�
TaskHandle_t LED0_Task_Handler;				//����������

void LED1_task(void* pvParameters);			//������
#define LED1_STK_SIZE 50					//���������ջ��С
#define LED1_TASK_PRIO 3 					//�����������ȼ�
TaskHandle_t LED1_Task_Handler;				//����������


void KEY_task(void* pvParameters);			//������
#define KEY_STK_SIZE 100					//���������ջ��С
#define KEY_TASK_PRIO 4 					//�����������ȼ�
TaskHandle_t KEY_Task_Handler;				//����������


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

	//����LED0����
	xTaskCreate((TaskFunction_t	)LED0_task,				//������
				(char*			)"LED0_task",			//��������
				(uint16_t		)LED0_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)LED0_TASK_PRIO,			//��������ȼ�
				(TaskHandle_t*	)&LED0_Task_Handler);	//������
				
	//����LED1����
	xTaskCreate((TaskFunction_t	)LED1_task,				//������
				(char*			)"LED1_task",			//��������
				(uint16_t		)LED1_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)LED1_TASK_PRIO,			//��������ȼ�
				(TaskHandle_t*	)&LED1_Task_Handler);	//������


	//������������
	xTaskCreate((TaskFunction_t )KEY_task,				//������
				(char*			)"KEY_task",			//��������
				(uint16_t		)KEY_STK_SIZE,			//�����ջ��С
				(void*			)NULL,					//���ݸ�����Ĳ���
				(UBaseType_t	)KEY_TASK_PRIO, 		//��������ȼ�
				(TaskHandle_t*	)&KEY_Task_Handler);	//������


				
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
		printf("led0_task������%d��\r\n",i);
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
		printf("led1_task������%d��\r\n",i);
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

