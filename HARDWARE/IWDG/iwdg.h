#ifndef _IWDG_H_
#define _IWDG_H_
#include "sys.h"


//��ʼ���������Ź�
//pre:��Ƶ��:0~7(ֻ�е�3λ��Ч!) rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^pre. ���ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ����㣨��ţ�:Tout=((4*2^prer)*rlr)/32 (ms)
void IWDG_Init(u8 pre,u16 rlr);

//ι��
void IWDG_Feed(void);


#endif
