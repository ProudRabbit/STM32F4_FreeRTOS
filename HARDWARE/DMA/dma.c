#include "dma.h"



//DMAx �ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8 λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA ������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA ͨ��ѡ��, @ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{
	DMA_InitTypeDef DMA_Initstructure;
	if((u32)DMA_Streamx > (u32)DMA2)	//�жϵ�ǰstream����DMA1����DMA2
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	}
	else
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);		//�ȴ�DMA��������

	/*����DMA*/
	DMA_Initstructure.DMA_Channel = chx;			//ͨ��ѡ��
	DMA_Initstructure.DMA_PeripheralBaseAddr = par;	//�����ַ
	DMA_Initstructure.DMA_Memory0BaseAddr = mar;	//�洢����ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	//�洢��������ģʽ
	DMA_Initstructure.DMA_BufferSize = ndtr;	//���ݴ�����
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//���������ģʽ
	DMA_Initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//�洢������ģʽ
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		//�洢�����ݳ��ȣ�8λ
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݳ��ȣ�8λ
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;						//��ͨģʽ
	DMA_Initstructure.DMA_Priority = DMA_Priority_Medium;				//�е����ȼ�
	DMA_Initstructure.DMA_FIFOMode = DMA_FIFOMode_Disable;				//����FIFOģʽ
	DMA_Initstructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;		//FIFO��ֵ
	DMA_Initstructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//�洢��ͻ�����δ���
	DMA_Initstructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//����ͻ�����δ���
	DMA_Init(DMA_Streamx, &DMA_Initstructure);							//��ʼ��DMA
	
}

//����һ�� DMA ����
//DMA_Streamx:DMA ������,DMA1_Stream0~7/DMA2_Stream0~7
//ndtr:���ݴ�����
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);			//�ر�DMA����
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);		//�ȴ�DMA��������
	DMA_SetCurrDataCounter(DMA_Streamx, ndtr);		//����DMA���ݴ�����
	DMA_Cmd(DMA_Streamx, ENABLE);			//����DMA����
	
}

