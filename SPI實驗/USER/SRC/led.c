#include "led.h"
#include "delay.h"
void LED_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	LED_RED_OFF;
	LED_YELLOW_OFF;
	LED_BLUE_OFF;
	LED_GREEN_OFF;
}

void Led_Show(void)
{
	LED_RED_ON;
	delay_ms(100);
	LED_YELLOW_ON;
	delay_ms(100);
	LED_BLUE_ON;
	delay_ms(100);
	LED_GREEN_ON;
	delay_ms(100);
	LED_RED_OFF;
	delay_ms(100);
	LED_YELLOW_OFF;
	delay_ms(100);
	LED_BLUE_OFF;
	delay_ms(100);
	LED_GREEN_OFF;
	delay_ms(100);
}
