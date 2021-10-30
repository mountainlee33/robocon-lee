/*
 * @Author: your name
 * @Date: 2021-04-03 16:21:37
 * @LastEditTime: 2021-04-04 22:15:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensorF4 - 3modif\USER\SRC\param.c
 */
#include "param.h"


PRESENSOR presensor[SensorNum];

//压力传感器ID参数初始化
void presensor_Init(void)
{
	{ //0号参数初始化
		presensor[0].constpre.number = 0;
		presensor[0].pressure = 0;
		presensor[0].constpre.Weight_Peel = 0;
		presensor[0].constpre.GapValue = 217.8712049;
		
		//io口配置
		presensor[0].pregpio.GPIO_DOUT = GPIOA;
		presensor[0].pregpio.Pin_DOUT = GPIO_Pin_2;
		presensor[0].pregpio.HX711_DOUT = PAin(2);

		presensor[0].pregpio.GPIO_SCK = GPIOB;
		presensor[0].pregpio.Pin_SCK = GPIO_Pin_4;
		presensor[0].pregpio.HX711_SCK = PBout(4);
		
		//状态初始化
		presensor[0].preflag.enable = 0;
		presensor[0].preflag.SetZero = 0;
		presensor[0].preflag.DOUT_error = 0;
	}

	{ //1号参数初始化
		presensor[1].constpre.number = 1;
		presensor[1].pressure = 0;
		presensor[1].constpre.Weight_Peel = 0;
		presensor[1].constpre.GapValue = 251.8963553;

		presensor[1].pregpio.GPIO_DOUT = GPIOB;
		presensor[1].pregpio.Pin_DOUT = GPIO_Pin_6;
		presensor[1].pregpio.HX711_DOUT = PBin(6);

		presensor[1].pregpio.GPIO_SCK = GPIOB;
		presensor[1].pregpio.Pin_SCK = GPIO_Pin_5;
		presensor[1].pregpio.HX711_SCK = PBout(5);

		presensor[1].preflag.enable = 0;
		presensor[1].preflag.SetZero = 0;
		presensor[1].preflag.DOUT_error = 0;
	}

	{ //2号参数初始化
		presensor[2].constpre.number = 2;
		presensor[2].pressure = 0;
		presensor[2].constpre.Weight_Peel = 0;
		presensor[2].constpre.GapValue = GapValue_2;

		presensor[2].pregpio.GPIO_DOUT = GPIOB;
		presensor[2].pregpio.Pin_DOUT = GPIO_Pin_7;
		presensor[2].pregpio.HX711_DOUT = PBin(7);

		presensor[2].pregpio.GPIO_SCK = GPIOB;
		presensor[2].pregpio.Pin_SCK = GPIO_Pin_8;
		presensor[2].pregpio.HX711_SCK = PBout(8);

		presensor[2].preflag.enable = 0;
		presensor[2].preflag.SetZero = 0;
		presensor[2].preflag.DOUT_error = 0;
	}
}
