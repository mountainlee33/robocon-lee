#ifndef _PARAM_H
#define _PARAM_H

#include "stdint.h"
#include "stm32f4xx.h"


struct PRESENSOR{
    int number;
    int pressure;   
int Weight_Peel;
int HX711_Buffer;	
	  double GapValue;
		uint32_t IDBACK;
    uint32_t IDSELF;
		volatile unsigned long Pout;
		volatile unsigned long Pin;
	  uint16_t HX711_SCK;
	  uint16_t HX711_DOUT;
		GPIO_TypeDef* GPIO_DOUT; 
		GPIO_TypeDef* GPIO_SCK; 

};

extern struct PRESENSOR presensor[3];



//压力传感器系数
#define GapValue_0 252.24
#define GapValue_1 252.4434
#define GapValue_2 218.0136


void presensor_Init(void);


#endif
    GPIO_Init(GPIO_DOUT, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIO_SCK,Pin_SCK);							//初始化设置为0
}

//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read(struct PRESENSOR *presensor)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	presensor->HX711_DOUT=1; //如果F4读到的输入为1
	Delay_us(1);
  	presensor->HX711_SCK=0; //给SCK输出低电平？
  	count=0; 
  	while(presensor->HX711_DOUT); //读取到0开始进行计算
  	for(i=0;i<24;i++)
	{ 
	  	presensor->HX711_SCK=1; 
	  	count=count<<1; 
		Delay_us(1);
		presensor->HX711_SCK=0; 
	  	if(presensor->HX711_DOUT)
			count++; 
		Delay_us(1);
	} 
 	presensor->HX711_SCK=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	Delay_us(1);
	presensor->HX711_SCK=0;  
	return(count);
}


//****************************************************
//获取毛皮重量
//****************************************************
void Get_Peel(struct PRESENSOR *presensor)
{
	presensor->Weight_Peel = HX711_Read();	
} 

//第一次读到的值认为是毛皮，后面在while循环中读的是实物
void Get_Weight(struct PRESENSOR *presensor)
{
	presensor->HX711_Buffer = HX711_Read();
	if(presensor->HX711_Buffer >presensor->Weight_Peel)			
	{
		delta = presensor->HX711_Buffer - presensor->Weight_Peel;				//获取实物的AD采样数值。
    presensor->pressure=(u32)(delta/(presensor->GapValue));
	}
}

