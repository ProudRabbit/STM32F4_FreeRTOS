#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"

int main(void)
{
	delay_init(168);
	LED_Init();
	KEY_Init();
	uart_init(115200);
	BEEP_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	u8 len,t;
	char *str1 ="Ã«Ã«ÊÇ²Ë¼¦";
	char *str2 ="Ã«Ã«²»ÊÇ²Ë¼¦";
	while(1)
	{	
		if((USART_RX_STA & 0x8000))
		{
			len = USART_RX_STA&0x3fff;
			printf("\r\nÄã·¢ËÍµÄÃüÁîÎª:\r\n");
			
			for(t=0;t<len;t++)
			{
				if(USART_RX_BUF[t]!=(*str1)&&USART_RX_BUF[t]!=(*str2))
					break;
				else
					str1++;
					str2++;
				USART_SendData(USART1,USART_RX_BUF[t]);
				while((USART1->SR&0X40)==0);
			}
			BEEP = !BEEP;
			str1 = "Ã«Ã«ÊÇ²Ë¼¦";
			str2 = "Ã«Ã«²»ÊÇ²Ë¼¦";
			printf("\r\n");
			USART_RX_STA =0;
		}
		delay_ms(100);
	}
}


