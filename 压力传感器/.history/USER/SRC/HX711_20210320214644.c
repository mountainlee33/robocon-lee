/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-03-20 21:21:25
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\SRC\HX711.c
 */
#include "HX711.h"

u32 HX711_Buffer;
u32 Weight_Peel;
u32 delta;
u32 Weight_Shiwu;
u8 Flag_Error = 0;

//校准参数
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
//手动调整
#define GapValue 252.5

void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_SCK | RCC_DOUT,ENABLE);	//使能PA端口时钟

	//HX711_SCK,PA0
	GPIO_InitStructure.GPIO_Pin = Pin_SCK;				// 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIO_SCK, &GPIO_InitStructure);					//根据设定参数初始化GPIOB
	
	//HX711_DOUT，传感器的out是我们的in,
	//PA1
    GPIO_InitStructure.GPIO_Pin = Pin_DOUT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//单片机其实读取的是HX711转化后的数字量，不应该用模拟输入
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
    GPIO_Init(GPIO_DOUT, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIO_SCK,Pin_SCK);							//初始化设置为0
}

//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; //如果F4读到的输入为1
	Delay_us(1);
  	HX711_SCK=0; //给SCK输出低电平？
  	count=0; 
  	while(HX711_DOUT); //读取到0开始进行计算
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		Delay_us(1);
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
		Delay_us(1);
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay_us(1);
	HX711_SCK=0;  
	return(count);
}


//****************************************************
//获取毛皮重量
//****************************************************
void Get_Peel(void)
{
	Weight_Peel = HX711_Read();	
} 

//第一次读到的值认为是毛皮，后面在while循环中读的是实物
u32 Get_Weight(float GapValue)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Peel)			
	{
		delta = HX711_Buffer - Weight_Peel;				//获取实物的AD采样数值。

    Weight_Shiwu=(u32)(delta/GapValue);
	}
	return Weight_Shiwu;
}

