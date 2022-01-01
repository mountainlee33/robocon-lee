#include "cylinder.h"
//与串口三配置
const unsigned char CYL_table[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, //0-F
								   0xFF, 0x00};	//通过控制接口高低电平来控制电磁阀进气方向

void CYLINDE_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	Set_CYLCLK;
	Reset_CYLDATA;
}

void CYLINDE_CONTROL (u8 Data)
{
	unsigned char i;
	if( Data > 0xFF)
		Data = 0XFF; //防止输入形参超出LED_table数组，造成非法访问
	for (i = 0; i<8; i++)
	{
		Reset_CYLCLK;
		if (Data & 0x01)
			Set_CYLDATA;
		else
			Reset_CYLDATA;
		Set_CYLCLK;
		Data >>= 1;
	}
}
