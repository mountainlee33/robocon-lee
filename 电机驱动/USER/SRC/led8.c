#include "led8.h"
const unsigned char LED_table[]={0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E, //0-F
		            0xFF,0x00};//全亮，全灭
/*
****************************************
* Name : Led8_configuration
* Describe : 初始化数码管
* Input : None
* Output : None
* Return : None
****************************************
*/
void Led8_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
  
	Set_LED8CLK();
	Reset_LED8DATA();
}

/*
****************************************
* Name : Led8DisData
* Describe : 在数码管上显示数字
* Input : Data  想要在数码管上显示的十六
								进制数字，范围从0到F，还
								可以全亮全灭
* Output : None
* Return : None
****************************************
*/		
void Led8_show(unsigned char Data)
{
	unsigned char i,val;
	if(Data>17)
		Data=17;			//防止输入形参超出LED_table数组，造成非法访问
	val=LED_table[Data];
	for(i=0;i<8;i++)
	{
//		Reset_LED8CLK();
		if(val & 0x01)
			Set_LED8DATA();
		else 
//			Reset_LED8DATA();
		Set_LED8CLK();
		val>>=1;
	}	
}


