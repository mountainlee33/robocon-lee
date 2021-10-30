#include "param.h"

struct PRESENSOR presensor[3];


//压力传感器ID参数初始化
void presensor_Init(void)
{
presensor[0].number = 0;
	presensor[0].pressure=0;
	presensor[0].GapValue = 217.8712049;
	
	presensor[0].GPIO_DOUT = GPIOA;
	presensor[0].Pin_DOUT = GPIO_Pin_2;
	presensor[0].HX711_DOUT = PAin(2);	
	
	presensor[0].GPIO_SCK = GPIOA;
	presensor[0].Pin_SCK = GPIO_Pin_3;
	presensor[0].HX711_SCK = PAout(3);
		
	
	
	presensor[1].number = 1;
	presensor[1].pressure=0;
	presensor[1].GapValue = 251.8963553;
	
	presensor[1].GPIO_DOUT = GPIOB;
	presensor[1].Pin_DOUT = GPIO_Pin_0;
  presensor[1].HX711_DOUT = PBin(0);	

	presensor[1].GPIO_SCK = GPIOB;
	presensor[1].Pin_SCK = GPIO_Pin_1;
	presensor[1].HX711_SCK = PBout(1);
	
	
	
	presensor[2].number = 2;
	presensor[2].pressure=0;
	presensor[2].GapValue = GapValue_2;

	presensor[2].GPIO_DOUT = GPIOB;
	presensor[2].Pin_DOUT = GPIO_Pin_5;
	presensor[2].HX711_DOUT = PBin(5);	
	
	presensor[2].GPIO_SCK = GPIOB;
	presensor[2].Pin_SCK = GPIO_Pin_6;
	presensor[2].HX711_SCK = PBout(6);


}



