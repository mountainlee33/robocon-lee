#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "includes.h"

//#define LED_POWER_OFF 			GPIOA->BSRRL = GPIO_Pin_15
#define LED_BLUE_OFF    	  GPIOA->BSRRL = GPIO_Pin_6
#define LED_GREEN_OFF  			GPIOA->BSRRL = GPIO_Pin_7
#define LED_YELLOW_OFF  		GPIOA->BSRRL = GPIO_Pin_5
#define LED_RED_OFF  				GPIOA->BSRRL = GPIO_Pin_4

//#define LED_POWER_ON 				GPIOA->BSRRH = GPIO_Pin_15
#define LED_BLUE_ON   			GPIOA->BSRRH = GPIO_Pin_6
#define LED_GREEN_ON   			GPIOA->BSRRH = GPIO_Pin_7
#define LED_YELLOW_ON   		GPIOA->BSRRH = GPIO_Pin_5
#define LED_RED_ON   				GPIOA->BSRRH = GPIO_Pin_4

//#define LED_POWER_TOGGLE		GPIOA->ODR ^= GPIO_Pin_15
#define LED_RED_TOGGLE			GPIOA->ODR ^= GPIO_Pin_4
#define LED_YELLOW_TOGGLE 	GPIOA->ODR ^= GPIO_Pin_5
#define LED_BLUE_TOGGLE			GPIOA->ODR ^= GPIO_Pin_6
#define LED_GREEN_TOGGLE		GPIOA->ODR ^= GPIO_Pin_7

void LED_Configuration(void);
void Led_Show(void);
#endif
