#include "sys.h"
#include "delay.h" 

int main(void)
{ 
	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz
	delay_init(168);		//初始化延时函数
	while(1)
	{
		delay_ms(500);
	}
}
