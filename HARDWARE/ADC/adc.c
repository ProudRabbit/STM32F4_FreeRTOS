#include "adc.h"
#include "delay.h"


//ADC初始化，使用规则通道
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitstructure;
    ADC_InitTypeDef ADC_Initstructure;

    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				//使能AHB1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);					//使能ADC3时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;										//初始化F6为模拟输入ADC3_IN4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;								//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//无上下拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);				//复位ADC3
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);				//复位ADC3结束

    ADC_CommonInitstructure.ADC_Mode = ADC_Mode_Independent;            //使用独立模式
    ADC_CommonInitstructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //不使能DMA
    ADC_CommonInitstructure.ADC_Prescaler = ADC_Prescaler_Div4;                 //预分频4分频 84/4 = 21MHz ADC最好不要超过36MHz
    ADC_CommonInitstructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段延迟5个时钟
    ADC_CommonInit(&ADC_CommonInitstructure);                                   //初始化

    //初始化ADC3相关参数
    ADC_Initstructure.ADC_Resolution = ADC_Resolution_12b;  //12位模式
    ADC_Initstructure.ADC_ScanConvMode = DISABLE;           //非扫描模式
    ADC_Initstructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发模式，使用软件触发
    ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;         //右对齐
    ADC_Initstructure.ADC_ContinuousConvMode = DISABLE;         //关闭连续转换
    ADC_Initstructure.ADC_NbrOfConversion = 1;                  //1个转换在规则序列中
    ADC_Init(ADC3, &ADC_Initstructure);                         //初始化ADC3

    ADC_Cmd(ADC3, ENABLE);        //启用ADC3转换
    
}


//获取ADC通道值
//ch为通道 0~16
//返回值：转换结果
u16 Get_Adc_Val(u8 ch)
{
    //设置ADC的规则序列，一个通道，采样时间
    ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(ADC3);       //使能ADC3的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));    //等待转换结束
    return ADC_GetConversionValue(ADC3);            //返回最近一次ADC规则组的转换结果
}


//获取times次ADC     ch通道的值，然后取平均
//ch为通道 0~16
//times：获取次数
//返回值：转换结果
u16 Get_Adc_AverageVal(u8 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += Get_Adc_Val(ch);
		delay_ms(5);
	}
	return temp_val/times;
}


