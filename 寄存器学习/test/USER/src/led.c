#include "led.h"

//初始化 pa4,pa5,pa6,pa7口，并使能时钟
void LED_Init(void)
{
    RCC->AHB1ENR  = 1 << 5; //将rcc寄存器的第五位 置1
    GPIO_Set(GPIOA, PIN4 | PIN5 | PIN6 | PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP,
             GPIO_SPEED_100M, GPIO_PUPD_PU);
}
