#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


//键值

#define KEY0_PRES 1		
#define KEY1_PRES 2
#define KEY2_PRES 3
#define WKUP_RPES 4


void KEY_Init(void);	//初始化IO
u8 KEY_Scan(u8 mode);	//按键扫描

#endif

