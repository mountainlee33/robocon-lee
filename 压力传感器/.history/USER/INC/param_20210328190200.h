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
