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
bool transfer_enable;

//压力传感器ID参数初始化
void presensor_Init(void)
{
	{ //0号参数初始化白板子6号
		presensor[0].constpre.number = 9;
		presensor[0].pressure = 0;
		presensor[0].constpre.Weight_Peel = 0;
		presensor[0].constpre.GapValue =GapValue_9;
		
		//io口配置
		presensor[0].pregpio.GPIO_DOUT = GPIOC;
		presensor[0].pregpio.Pin_DOUT = GPIO_Pin_1;
		presensor[0].pregpio.HX711_DOUT = PCin(1);

		presensor[0].pregpio.GPIO_SCK = GPIOC;
		presensor[0].pregpio.Pin_SCK = GPIO_Pin_0;
		presensor[0].pregpio.HX711_SCK = PCout(0);
		
		//状态初始化
		presensor[0].preflag.enable = 0;
		presensor[0].preflag.SetZero = 0;
		presensor[0].preflag.DOUT_error = 0;
	}

	{ //1号参数初始化白板子1号
		presensor[1].constpre.number = 10;
		presensor[1].pressure = 0;
		presensor[1].constpre.Weight_Peel = 0;
		presensor[1].constpre.GapValue = GapValue_10;

		presensor[1].pregpio.GPIO_DOUT = GPIOB;
		presensor[1].pregpio.Pin_DOUT = GPIO_Pin_9;
		presensor[1].pregpio.HX711_DOUT = PBin(9);

		presensor[1].pregpio.GPIO_SCK = GPIOB;
		presensor[1].pregpio.Pin_SCK = GPIO_Pin_8;
		presensor[1].pregpio.HX711_SCK = PBout(8);

		presensor[1].preflag.enable = 0;
		presensor[1].preflag.SetZero = 0;
		presensor[1].preflag.DOUT_error = 0;
	}

	{ //2号参数初始化白板子2号
		presensor[2].constpre.number = 5;
		presensor[2].pressure = 0;
		presensor[2].constpre.Weight_Peel = 0;
		presensor[2].constpre.GapValue = GapValue_5;

		presensor[2].pregpio.GPIO_DOUT = GPIOB;
		presensor[2].pregpio.Pin_DOUT = GPIO_Pin_3;
		presensor[2].pregpio.HX711_DOUT = PBin(3);

		presensor[2].pregpio.GPIO_SCK = GPIOD;
		presensor[2].pregpio.Pin_SCK = GPIO_Pin_2;
		presensor[2].pregpio.HX711_SCK = PDout(2);

		presensor[2].preflag.enable = 0;
		presensor[2].preflag.SetZero = 0;
		presensor[2].preflag.DOUT_error = 0;
	}
	{ //3号参数初始化白板子3号
		presensor[3].constpre.number = 5;
		presensor[3].pressure = 0;
		presensor[3].constpre.Weight_Peel = 0;
		presensor[3].constpre.GapValue = GapValue_5;

		presensor[3].pregpio.GPIO_DOUT = GPIOC;
		presensor[3].pregpio.Pin_DOUT = GPIO_Pin_12;
		presensor[3].pregpio.HX711_DOUT = PCin(12);

		presensor[3].pregpio.GPIO_SCK = GPIOC;
		presensor[3].pregpio.Pin_SCK = GPIO_Pin_11;
		presensor[3].pregpio.HX711_SCK = PCout(11);

		presensor[3].preflag.enable = 0;
		presensor[3].preflag.SetZero = 0;
		presensor[3].preflag.DOUT_error = 0;
	}
}
