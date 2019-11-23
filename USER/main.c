#include "include.h"


void start_task(void* pvParameters);		//任务函数
#define START_STK_SIZE 128					//定义任务堆栈大小
#define START_TASK_PRIO 1 					//定义任务优先级
TaskHandle_t StartTask_Handler;				//定义任务句柄


void LED0_task(void* pvParameters);			//任务函数
#define LED0_STK_SIZE 50					//定义任务堆栈大小
#define LED0_TASK_PRIO 2 					//定义任务优先级
TaskHandle_t LED0_Task_Handler;				//定义任务句柄

void List_task(void* pvParameters);			//任务函数
#define List_STK_SIZE 128					//定义任务堆栈大小
#define List_TASK_PRIO 3 					//定义任务优先级
TaskHandle_t List_Task_Handler;				//定义任务句柄


void KEY_task(void* pvParameters);			//任务函数
#define KEY_STK_SIZE 100					//定义任务堆栈大小
#define KEY_TASK_PRIO 4 					//定义任务优先级
TaskHandle_t KEY_Task_Handler;				//定义任务句柄

List_t TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;




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
				
	//创建List任务
	xTaskCreate((TaskFunction_t	)List_task,				//任务函数
				(char*			)"List_task",			//任务名称
				(uint16_t		)List_STK_SIZE,			//任务堆栈大小
				(void*			)NULL, 					//传递给任务的参数
				(UBaseType_t	)List_TASK_PRIO,		//任务的优先级
				(TaskHandle_t*	)&List_Task_Handler);	//任务句柄


				
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
	vListInitialise(&TestList);			//初始化列表
	vListInitialiseItem(&ListItem1);	//初始化列表项
	vListInitialiseItem(&ListItem2);	//初始化列表项
	vListInitialiseItem(&ListItem3);	//初始化列表项
	
	ListItem1.xItemValue = 30;
	ListItem2.xItemValue = 60;
	ListItem3.xItemValue = 50;
	
	//第二步：打印列表和其他列表项的地址
	printf("/*******************列表和列表项地址*******************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//等待KEY_UP键按下

	//第三步：向列表TestList添加列表项ListItem1，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem1);		//插入列表项ListItem1
	printf("/******************添加列表项ListItem1*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//等待KEY_UP键按下
	
		//第四步：向列表TestList添加列表项ListItem2，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem2);	//插入列表项ListItem2
	printf("/******************添加列表项ListItem2*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//等待KEY_UP键按下
	
	//第五步：向列表TestList添加列表项ListItem3，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem3);	//插入列表项ListItem3
	printf("/******************添加列表项ListItem3*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//等待KEY_UP键按下
	
	//第六步：删除ListItem2，并通过串口打印所有列表项中成员变量pxNext和
	//pxPrevious的值，通过这两个值观察列表项在列表中的连接情况。
	uxListRemove(&ListItem2);						//删除ListItem2
	printf("/******************删除列表项ListItem2*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);					//等待KEY_UP键按下
	
	//第七步：删除ListItem2，并通过串口打印所有列表项中成员变量pxNext和
	//pxPrevious的值，通过这两个值观察列表项在列表中的连接情况。
	TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex向后移一项，这样pxIndex就会指向ListItem1。
	vListInsertEnd(&TestList,&ListItem2);				//列表末尾添加列表项ListItem2
	printf("/***************在末尾添加列表项ListItem2***************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************结束**************************/\r\n\r\n\r\n");

	while(1)
	{
		LED1=!LED1;
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}



