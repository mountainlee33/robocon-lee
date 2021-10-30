/*
 * @Author: your name
 * @Date: 2021-03-28 16:03:28
 * @LastEditTime: 2021-04-04 20:26:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor3\USER\SRC\HX711.c
 */

#include "HX711.h"

s32 DELTA = 0;
int num;

void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE); //使能PA端口时钟

	//HX711_DOUT
	GPIO_InitStructure.GPIO_Pin = presensor[0].pregpio.Pin_DOUT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	  //单片机其实读取的是HX711转化后的数字量，不应该用模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  //输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(presensor[0].pregpio.GPIO_DOUT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = presensor[1].pregpio.Pin_DOUT;
	GPIO_Init(presensor[1].pregpio.GPIO_DOUT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = presensor[2].pregpio.Pin_DOUT;
	GPIO_Init(presensor[2].pregpio.GPIO_DOUT, &GPIO_InitStructure);

	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = presensor[0].pregpio.Pin_SCK; // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			   //IO口速度为50MHz
	GPIO_Init(presensor[0].pregpio.GPIO_SCK, &GPIO_InitStructure); //根据设定参数初始化GPIOB
	GPIO_InitStructure.GPIO_Pin = presensor[0].pregpio.Pin_SCK;	   // 端口配置
	GPIO_Init(presensor[0].pregpio.GPIO_SCK, &GPIO_InitStructure); //根据设定参数初始化GPIOB

	GPIO_InitStructure.GPIO_Pin = presensor[2].pregpio.Pin_SCK;	   // 端口配置
	GPIO_Init(presensor[2].pregpio.GPIO_SCK, &GPIO_InitStructure); //根据设定参数初始化GPIOB

	GPIO_SetBits(presensor[0].pregpio.GPIO_SCK, presensor[0].pregpio.Pin_SCK); //初始化设置为0
	GPIO_SetBits(presensor[1].pregpio.GPIO_SCK, presensor[1].pregpio.Pin_SCK); //初始化设置为0
	GPIO_SetBits(presensor[2].pregpio.GPIO_SCK, presensor[2].pregpio.Pin_SCK); //初始化设置为0
}

//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read(PRESENSOR *Presensor) //增益128
{
	unsigned long count;
	unsigned char i;
	*(Presensor->pregpio.HX711_DOUT) = 1; //如果F4读到的输入为1
	Delay_us(1);
	*(Presensor->pregpio.HX711_SCK) = 0; //给SCK输出低电平？
	count = 0;
	while (*(Presensor->pregpio.HX711_DOUT))
		; //读取到0开始进行计算
	for (i = 0; i < 24; i++)
	{
		*(Presensor->pregpio.HX711_SCK) = 1;
		count = count << 1;
		Delay_us(1);
		*(Presensor->pregpio.HX711_SCK) = 0;
		if (*(Presensor->pregpio.HX711_DOUT))
			count++;
		Delay_us(1);
	}
	*(Presensor->pregpio.HX711_SCK) = 1;
	count = count ^ 0x800000; //第25个脉冲下降沿来时，转换数据
	Delay_us(1);
	*(Presensor->pregpio.HX711_SCK) = 0;
	return (count);
}

//****************************************************
//获取毛皮重量
//****************************************************
//第一次读到的值认为是毛皮，后面在while循环中读的是实物
void Get_Peel(void)
{
	for(num=0;num<3;num++)
	{
	if (presensor[num].enable == 1)
	{
		presensor[num].constpre.Weight_Peel = HX711_Read(&presensor[num]);
	}
	}

}

//第一次读到的值认为是毛皮，后面在while循环中读的是实物
void Get_Weight(void)
{
	int num=0;
	for(num=0;num<3;num++)
	{
		if(presensor[num].enable ==1)
		{
			presensor[num].constpre.HX711_Buffer = HX711_Read(&presensor[num]);
			DELTA = presensor[num].constpre.HX711_Buffer-presensor[num].constpre.Weight_Peel;
			presensor[num].pressure = (DELTA/presensor[num].constpre.GapValue);
		}
	}
}

void Set_Zero(void)
{
	int num=0;
	for(num=0;num<3;num++)
	{
		if(presensor[num].SetZero == 1)
		{
			presensor[num].pressure = presensor[num].constpre.HX711_Buffer = presensor[num].constpre.Weight_Peel =0;
			Delay_ms(10);
		}
	}
	
}