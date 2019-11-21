#ifndef _DMA_H_
#define _DAC_H_

#include "sys.h"

//DMAx �ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8 λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA ������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA ͨ��ѡ��, @ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);


//����һ�� DMA ����
//DMA_Streamx:DMA ������,DMA1_Stream0~7/DMA2_Stream0~7
//ndtr:���ݴ�����
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);

#endif


