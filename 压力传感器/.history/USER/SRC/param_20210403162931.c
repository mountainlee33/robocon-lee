/*
 * @Author: your name
 * @Date: 2021-04-03 10:33:17
 * @LastEditTime: 2021-04-03 16:28:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \USERd:\桌面\pressureF1 - 3MODIF\MyCode\param\param.c
 */
#include "param.h"


PRESENSOR presensor[3];

//压力传感器ID参数初始化
void presensor_Init(void)
{
	{ //0号参数初始化
		presensor[0].number = 0;
		presensor[0].pressure = 0;
		presensor[0].Weight_Peel = 0;
		presensor[0].GapValue = 217.8712049;
		//io口配置
		presensor[0].pregpio.GPIO_DOUT = GPIOA;
		presensor[0].pregpio.Pin_DOUT = GPIO_Pin_2;
		presensor[0].pregpio.HX711_DOUT = PAin(2);

		presensor[0].pregpio.GPIO_SCK = GPIOB;
		presensor[0].pregpio.Pin_SCK = GPIO_Pin_4;
		presensor[0].pregpio.HX711_SCK = PBout(4);
		//状态初始化
		presensor[0].enable = 0;
		presensor[0].SetZero = 0;
	}

	{ //1号参数初始化
		presensor[1].number = 1;
		presensor[1].pressure = 0;
		presensor[1].Weight_Peel = 0;
		presensor[1].GapValue = 251.8963553;

		presensor[1].pregpio.GPIO_DOUT = GPIOB;
		presensor[1].pregpio.Pin_DOUT = GPIO_Pin_0;
		presensor[1].pregpio.HX711_DOUT = PBin(0);

		presensor[1].pregpio.GPIO_SCK = GPIOB;
		presensor[1].pregpio.Pin_SCK = GPIO_Pin_1;
		presensor[1].pregpio.HX711_SCK = PBout(1);

		presensor[1].enable = 0;
		presensor[1].SetZero = 0;
	}

	{ //2号参数初始化
		presensor[2].number = 2;
		presensor[2].pressure = 0;
		presensor[2].Weight_Peel = 0;
		presensor[2].GapValue = GapValue_2;

		presensor[2].pregpio.GPIO_DOUT = GPIOB;
		presensor[2].pregpio.Pin_DOUT = GPIO_Pin_5;
		presensor[2].pregpio.HX711_DOUT = PBin(5);

		presensor[2].pregpio.GPIO_SCK = GPIOB;
		presensor[2].pregpio.Pin_SCK = GPIO_Pin_6;
		presensor[2].pregpio.HX711_SCK = PBout(6);

		presensor[2].enable = 0;
		presensor[2].SetZero = 0;
	}
}
