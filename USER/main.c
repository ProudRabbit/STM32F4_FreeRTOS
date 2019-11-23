#include "include.h"


void start_task(void* pvParameters);		//������
#define START_STK_SIZE 128					//���������ջ��С
#define START_TASK_PRIO 1 					//�����������ȼ�
TaskHandle_t StartTask_Handler;				//����������


void LED0_task(void* pvParameters);			//������
#define LED0_STK_SIZE 50					//���������ջ��С
#define LED0_TASK_PRIO 2 					//�����������ȼ�
TaskHandle_t LED0_Task_Handler;				//����������

void List_task(void* pvParameters);			//������
#define List_STK_SIZE 128					//���������ջ��С
#define List_TASK_PRIO 3 					//�����������ȼ�
TaskHandle_t List_Task_Handler;				//����������


void KEY_task(void* pvParameters);			//������
#define KEY_STK_SIZE 100					//���������ջ��С
#define KEY_TASK_PRIO 4 					//�����������ȼ�
TaskHandle_t KEY_Task_Handler;				//����������

List_t TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;




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
				
	//����List����
	xTaskCreate((TaskFunction_t	)List_task,				//������
				(char*			)"List_task",			//��������
				(uint16_t		)List_STK_SIZE,			//�����ջ��С
				(void*			)NULL, 					//���ݸ�����Ĳ���
				(UBaseType_t	)List_TASK_PRIO,		//��������ȼ�
				(TaskHandle_t*	)&List_Task_Handler);	//������


				
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void LED0_task(void * pvParameters)
{
	while(1)
	{
		LED0 = 1;
		vTaskDelay(500);
		LED0 = 0;
		vTaskDelay(500);
	}
}


void List_task(void * pvParameters)
{
	vListInitialise(&TestList);			//��ʼ���б�
	vListInitialiseItem(&ListItem1);	//��ʼ���б���
	vListInitialiseItem(&ListItem2);	//��ʼ���б���
	vListInitialiseItem(&ListItem3);	//��ʼ���б���
	
	ListItem1.xItemValue = 30;
	ListItem2.xItemValue = 60;
	ListItem3.xItemValue = 50;
	
	//�ڶ�������ӡ�б�������б���ĵ�ַ
	printf("/*******************�б���б����ַ*******************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//�ȴ�KEY_UP������

	//�����������б�TestList����б���ListItem1����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem1);		//�����б���ListItem1
	printf("/******************����б���ListItem1*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//�ȴ�KEY_UP������
	
		//���Ĳ������б�TestList����б���ListItem2����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem2);	//�����б���ListItem2
	printf("/******************����б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//�ȴ�KEY_UP������
	
	//���岽�����б�TestList����б���ListItem3����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem3);	//�����б���ListItem3
	printf("/******************����б���ListItem3*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//�ȴ�KEY_UP������
	
	//��������ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	uxListRemove(&ListItem2);						//ɾ��ListItem2
	printf("/******************ɾ���б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//�ȴ�KEY_UP������
	
	//���߲���ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex�����һ�����pxIndex�ͻ�ָ��ListItem1��
	vListInsertEnd(&TestList,&ListItem2);				//�б�ĩβ����б���ListItem2
	printf("/***************��ĩβ����б���ListItem2***************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n\r\n\r\n");

	while(1)
	{
		LED1=!LED1;
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}



