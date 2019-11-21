#ifndef _IIC_H_
#define _IIC_H_

#include "sys.h"


//IO ��������
#define SDA_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
//PB9 ����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}
//PB9 ���ģʽ
//IO ��������
#define IIC_SCL PBout(8) //SCL
#define IIC_SDA PBout(9) //SDA
#define READ_SDA PBin(9) //���� SDA

//��ʼ�� IIC
void IIC_Init(void);
//���� IIC ��ʼ�ź�
void IIC_Start(void);
//���� IIC ֹͣ�ź�
void IIC_Stop(void);
//�ȴ�Ӧ���źŵ���
//����ֵ�� 1������Ӧ��ʧ��
// 0������Ӧ��ɹ�
u8 IIC_Wait_ACK(void);
//���� ACK Ӧ��
void IIC_ACK(void);
//������ ACK Ӧ��
void IIC_No_ACK(void);
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1 ��Ӧ��0 ��Ӧ��
void IIC_Send_Byte(u8 txd);
//��һ���ֽڣ�ack=1ʱ����ACK��ack=0ʱ����noack
u8 IIC_Read_Byte(unsigned char ack);



#endif

