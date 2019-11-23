#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 256					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������


void Interrupt_task(void* pvParameters);			//������
#define Interrupt_STK_SIZE 256					//���������ջ��С
#define Interrupt_TASK_PRIO 2 					//�����������ȼ�
TaskHandle_t Interrupt_Task_Handler;				//����������


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	uart_init(115200);		//��ʼ������
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	TIM3_Init(10000-1, 8400-1);
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
	xTaskCreate((TaskFunction_t	)Interrupt_task,				//������
				(char*			)"Interrupt_task",			//��������
				(uint16_t		)Interrupt_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)Interrupt_TASK_PRIO,			//��������ȼ�
				(TaskHandle_t*	)&Interrupt_Task_Handler);	//������
							
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
			printf("�ر��жϡ�������������\r\n");
			portDISABLE_INTERRUPTS();
			delay_xms(5000);
			portENABLE_INTERRUPTS();
			printf("���жϡ���������������\r\n");
		}

		LED0=~LED0;
        vTaskDelay(1000);
	}
}




