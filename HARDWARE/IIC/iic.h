#ifndef _IIC_H_
#define _IIC_H_

#include "sys.h"


//IO 方向设置
#define SDA_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
//PB9 输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}
//PB9 输出模式
//IO 操作函数
#define IIC_SCL PBout(8) //SCL
#define IIC_SDA PBout(9) //SDA
#define READ_SDA PBin(9) //输入 SDA

//初始化 IIC
void IIC_Init(void);
//产生 IIC 起始信号
void IIC_Start(void);
//产生 IIC 停止信号
void IIC_Stop(void);
//等待应答信号到来
//返回值： 1，接收应答失败
// 0，接收应答成功
u8 IIC_Wait_ACK(void);
//产生 ACK 应答
void IIC_ACK(void);
//不产生 ACK 应答
void IIC_No_ACK(void);
//IIC发送一个字节
//返回从机有误应答
//1 有应答，0 无应答
void IIC_Send_Byte(u8 txd);
//读一个字节，ack=1时发送ACK，ack=0时发送noack
u8 IIC_Read_Byte(unsigned char ack);



#endif

