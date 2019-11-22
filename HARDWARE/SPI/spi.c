#include "spi.h"



//以下是 SPI 模块的初始化代码，配置成主机模式
//SPI 口初始化
//这里针是对 SPI1 的初始化
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//GPIOFB3,4,5 初始化设置: 复用功能输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PB3 4 5 复用为SPI1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	//这里只针对 SPI 口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位 SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位 SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//数据大小
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//空闲为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//第二个跳变沿捕获数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS 信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//预分频 256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从 MSB 位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC 值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);		//初始化SPI1

	SPI_Cmd(SPI1, ENABLE);		//使能SPI1
	SPI1_ReadWriteByte(0xff);		//启动SPI传输
	
}

//SPI1 速度设置函数
//SPI 速度=fAPB2/分频系数
//入口参数范围： @ref SPI_BaudRate_Prescaler
//SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2 时钟一般为 84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1 &= 0XFFC7;	//位 3-5 清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置 SPI1 速度
	SPI_Cmd(SPI1,ENABLE);	//使能 SPI1
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);	//等待发送区空
	SPI_I2S_SendData(SPI1, TxData);		//通过外设 SPI1 发送一个 byte 数据
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);	//等待接收完成
	return SPI_I2S_ReceiveData(SPI1);	//返回通过 SPI1 最近接收的数据
}

