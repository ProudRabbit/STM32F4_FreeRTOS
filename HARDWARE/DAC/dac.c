#include "dac.h"

//DACͨ��һPA4��ʼ��
void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//�����ô���ʹ��TEN1=0;
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;	//�����ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;	//���η�ֵ����
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//�������ر�
	DAC_Init(DAC_Channel_1, &DAC_InitType); 	//��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1,ENABLE);	//ʹ��DACͨ��1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);	//12λ�Ҷ������ݸ�ʽ
}


//����DACͨ��1(PA4)�����ѹ
//val��0~3300��Ӧ0~3.3V
void Dac1_Set_Val(u16 val)
{
	double temp = val;
	temp /=1000;
	temp = temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R, temp);	//12λ�Ҷ������ݸ�ʽ
}




