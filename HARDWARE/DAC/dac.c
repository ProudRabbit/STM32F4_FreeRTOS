#include "dac.h"

//DAC通道一PA4初始化
void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//不适用触发使能TEN1=0;
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不适用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;	//屏蔽幅值设置
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//输出缓存关闭
	DAC_Init(DAC_Channel_1, &DAC_InitType); 	//初始化DAC通道1

	DAC_Cmd(DAC_Channel_1,ENABLE);	//使能DAC通道1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);	//12位右对齐数据格式
}


//设置DAC通道1(PA4)输出电压
//val：0~3300对应0~3.3V
void Dac1_Set_Val(u16 val)
{
	double temp = val;
	temp /=1000;
	temp = temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R, temp);	//12位右对齐数据格式
}




