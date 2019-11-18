#include "iwdg.h"


void IWDG_Init(u8 pre,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//д��0X5555ȡ��д����
	IWDG_SetPrescaler(pre);					//���÷�Ƶϵ��
	IWDG_SetReload(rlr);						//������װ��ֵ
	IWDG_ReloadCounter();						//��װ�ز���Ҳ������д��������
	IWDG_Enable();
}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}



