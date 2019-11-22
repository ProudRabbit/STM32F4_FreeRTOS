#ifndef _SPI_H_
#define _SPI_H_

#include "sys.h"

//������ SPI ģ��ĳ�ʼ�����룬���ó�����ģʽ
//SPI �ڳ�ʼ��
//�������Ƕ� SPI1 �ĳ�ʼ��
void SPI1_Init(void);

//SPI1 �ٶ����ú���
//SPI �ٶ�=fAPB2/��Ƶϵ��
//��ڲ�����Χ�� @ref SPI_BaudRate_Prescaler
//SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2 ʱ��һ��Ϊ 84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData);

#endif

