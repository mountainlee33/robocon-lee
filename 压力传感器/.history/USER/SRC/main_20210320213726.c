#include "main.h"

int main()
{
//	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	LED_Configuration();
	CAN1_Configuration();
	USART1_MyInit();
	Init_HX711pin();
	
	Get_Peel();				//³ÆÃ«Æ¤ÖØÁ¿
	Delay_ms(1000);
	Get_Peel();
	
	while(1)
	{
		Get_Weight();
		Delay_ms(1000);
	}
}

