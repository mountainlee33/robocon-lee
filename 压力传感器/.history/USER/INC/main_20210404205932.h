#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "delay.h"
#include "usart1.h"
#include "can1.h"
#include "param.h"
#include "led.h"
#include "HX711.h"

#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"


//START任务
#define START_TASK_PRIO            5
#define START_TASK_STK_SIZE        256
__align(8) OS_STK start_task_stk[START_TASK_STK_SIZE];
static void TaskStart(void *pdata);

//压力传感器读取数据任务
#define PRES_TASK_PRIO      10
#define PRES_TASK_STK_SIZE   256
__align(8) OS_STK pres_task_stk[PRES_TASK_STK_SIZE];
static void TaskPres(void *pdata);



OS_EVENT *RUN;

#endif

