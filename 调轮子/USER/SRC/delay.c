#include "delay.h"

/** 
  * @brief  ��ʱ������
  */
	
void delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/** 
  * @brief  ��ʱ��΢��
  */
void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/** 
  * @brief  ��ʱ��ָ��������
  */

void delay(u16 t)
{
	while(t--);
}
