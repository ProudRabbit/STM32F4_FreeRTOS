#ifndef _SPI_H_
#define _SPI_H_

#include "sys.h"

//以下是 SPI 模块的初始化代码，配置成主机模式
//SPI 口初始化
//这里针是对 SPI1 的初始化
void SPI1_Init(void);

//SPI1 速度设置函数
//SPI 速度=fAPB2/分频系数
//入口参数范围： @ref SPI_BaudRate_Prescaler
//SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2 时钟一般为 84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData);

#endif

