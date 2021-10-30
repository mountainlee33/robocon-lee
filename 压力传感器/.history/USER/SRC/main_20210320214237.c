/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-03-20 21:42:37
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\SRC\main.c
 */
#include "main.h"

u32 Pressure
int main()
{
//	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	LED_Configuration();
	CAN1_Configuration();
	USART1_MyInit();
	Init_HX711pin();
	
	Get_Peel();				//称毛皮重量
	Delay_ms(1000);
	Get_Peel();
	
	while(1)
	{
		Pressure = Get_Weight(GapValue_0);
		Delay_ms(1000);
	}
}

