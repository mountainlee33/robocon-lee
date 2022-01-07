#ifndef __LED_H
#define __LED_H

#include "sys.h"
#define LED0 (1<<4)
#define LED1 (1<<5)
#define LED2 PFout(6)
#define LED3 PFout(7)
void LED_Init(void);
#endif
