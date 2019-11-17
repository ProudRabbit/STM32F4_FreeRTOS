#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


//��ֵ

#define KEY0_PRES 1		
#define KEY1_PRES 2
#define KEY2_PRES 3
#define WKUP_RPES 4


void KEY_Init(void);	//��ʼ��IO
u8 KEY_Scan(u8 mode);	//����ɨ��

#endif

