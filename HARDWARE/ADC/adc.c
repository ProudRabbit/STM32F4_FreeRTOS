#include "adc.h"
#include "delay.h"


//ADC��ʼ����ʹ�ù���ͨ��
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitstructure;
    ADC_InitTypeDef ADC_Initstructure;

    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				//ʹ��AHB1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);					//ʹ��ADC3ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;										//��ʼ��F6Ϊģ������ADC3_IN4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;								//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//��������
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);				//��λADC3
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);				//��λADC3����

    ADC_CommonInitstructure.ADC_Mode = ADC_Mode_Independent;            //ʹ�ö���ģʽ
    ADC_CommonInitstructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //��ʹ��DMA
    ADC_CommonInitstructure.ADC_Prescaler = ADC_Prescaler_Div4;                 //Ԥ��Ƶ4��Ƶ 84/4 = 21MHz ADC��ò�Ҫ����36MHz
    ADC_CommonInitstructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶��ӳ�5��ʱ��
    ADC_CommonInit(&ADC_CommonInitstructure);                                   //��ʼ��

    //��ʼ��ADC3��ز���
    ADC_Initstructure.ADC_Resolution = ADC_Resolution_12b;  //12λģʽ
    ADC_Initstructure.ADC_ScanConvMode = DISABLE;           //��ɨ��ģʽ
    ADC_Initstructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ����ģʽ��ʹ���������
    ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;         //�Ҷ���
    ADC_Initstructure.ADC_ContinuousConvMode = DISABLE;         //�ر�����ת��
    ADC_Initstructure.ADC_NbrOfConversion = 1;                  //1��ת���ڹ���������
    ADC_Init(ADC3, &ADC_Initstructure);                         //��ʼ��ADC3

    ADC_Cmd(ADC3, ENABLE);        //����ADC3ת��
    
}


//��ȡADCͨ��ֵ
//chΪͨ�� 0~16
//����ֵ��ת�����
u16 Get_Adc_Val(u8 ch)
{
    //����ADC�Ĺ������У�һ��ͨ��������ʱ��
    ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(ADC3);       //ʹ��ADC3�����ת����������
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));    //�ȴ�ת������
    return ADC_GetConversionValue(ADC3);            //�������һ��ADC�������ת�����
}


//��ȡtimes��ADC     chͨ����ֵ��Ȼ��ȡƽ��
//chΪͨ�� 0~16
//times����ȡ����
//����ֵ��ת�����
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


