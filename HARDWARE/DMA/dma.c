#include "dma.h"



//DMAx 的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8 位数据宽度/存储器增量模式
//DMA_Streamx:DMA 数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA 通道选择, @ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef DMA_Initstructure;
	if((u32)DMA_Streamx > (u32)DMA2)	//判断当前stream属于DMA1还是DMA2
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	}
	else
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);		//等待DMA可以配置

	/*配置DMA*/
	DMA_Initstructure.DMA_Channel = chx;			//通道选择
	DMA_Initstructure.DMA_PeripheralBaseAddr = par;	//外设地址
	DMA_Initstructure.DMA_Memory0BaseAddr = mar;	//存储器地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	//存储器到外设模式
	DMA_Initstructure.DMA_BufferSize = ndtr;	//数据传输量
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设非增量模式
	DMA_Initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//存储器增量模式
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		//存储器数据长度：8位
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度：8位
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;						//普通模式
	DMA_Initstructure.DMA_Priority = DMA_Priority_Medium;				//中等优先级
	DMA_Initstructure.DMA_FIFOMode = DMA_FIFOMode_Disable;				//禁用FIFO模式
	DMA_Initstructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;		//FIFO阈值
	DMA_Initstructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输
	DMA_Initstructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//外设突发单次传输
	DMA_Init(DMA_Streamx, &DMA_Initstructure);							//初始化DMA
	
}

//开启一次 DMA 传输
//DMA_Streamx:DMA 数据流,DMA1_Stream0~7/DMA2_Stream0~7
//ndtr:数据传输量
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);			//关闭DMA传输
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);		//等待DMA可以配置
	DMA_SetCurrDataCounter(DMA_Streamx, ndtr);		//设置DMA数据传输量
	DMA_Cmd(DMA_Streamx, ENABLE);			//开启DMA传输
	
}

