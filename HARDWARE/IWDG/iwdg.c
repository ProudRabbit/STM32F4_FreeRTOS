#include "iwdg.h"


void IWDG_Init(u8 pre,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//写入0X5555取消写保护
	IWDG_SetPrescaler(pre);					//设置分频系数
	IWDG_SetReload(rlr);						//设置重装载值
	IWDG_ReloadCounter();						//重装载操作也会启动写保护功能
	IWDG_Enable();
}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}



