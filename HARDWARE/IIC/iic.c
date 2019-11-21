#include "iic.h"
#include "delay.h"


//初始化 IIC
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	IIC_SCL = 1;
	IIC_SDA = 1;
	
}


//产生 IIC 起始信号
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0;	//产生开始信号
	delay_us(4);
	IIC_SCL = 0;	//钳住SCL总线，准备发送和接受数据
}


//产生 IIC 停止信号
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;	//发送I2C总线结束信号
	delay_us(4);
}


//等待应答信号到来
//返回值： 1，接收应答失败
// 0，接收应答成功
u8 IIC_Wait_ACK(void)
{
	u8 ucErrtime = 0;
	SDA_IN();	//SDA设置为输入
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	while(READ_SDA)
	{
		ucErrtime++;
		if(ucErrtime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;
	return 0;
}

//产生 ACK 应答
void IIC_ACK(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}


//不产生 ACK 应答
void IIC_No_ACK(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}


//IIC发送一个字节
//返回从机有误应答
//1 有应答，0 无应答
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;	//拉低时钟开始传输数据
	for(t= 0;t<8;t++)
	{
		IIC_SDA = (txd&0x80)>>7;
		txd<<=1;
		delay_us(2);
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
		delay_us(2);
	}

}


//读一个字节，ack=1时发送ACK，ack=0时发送noack
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;
		receive <<=1;
		if(READ_SDA)
			receive++;
		delay_us(1);
	}
	if(!ack)
		IIC_No_ACK();
	else 
		IIC_ACK();
	return receive;
}


