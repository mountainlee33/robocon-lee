#include "beep.h"

//初始化pa8为输出口，并使能这个口的时钟
//蜂鸣器初始化
void BEEP_Init(void)
{
    RCC->AHB1ENR = 1 << 5;
    GPIO_Set(GPIOA, PIN8,GPIO_MODE_OUT, GPIO_OTYPE_PP,
             GPIO_SPEED_100M, GPIO_PUPD_PD);

    BEEP = 0;
}
