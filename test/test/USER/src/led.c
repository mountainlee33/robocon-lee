#include "led.h"

//��ʼ�� pa4,pa5,pa6,pa7�ڣ���ʹ��ʱ��
void LED_Init(void)
{
    RCC->AHB1ENR  = 1 << 5; //��rcc�Ĵ����ĵ���λ ��1
    GPIO_Set(GPIOA, PIN4 | PIN5 | PIN6 | PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP,
             GPIO_SPEED_100M, GPIO_PUPD_PU);
}
