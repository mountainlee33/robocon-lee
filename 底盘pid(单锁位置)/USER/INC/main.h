
#ifndef _MAIN_H
#define _MAIN_H

#include "includes.h"//任何文件必须引入
#include "delay.h"
#include "os_cpu.h"
#include "ucos_ii.h"
#include "can1.h"
#include "can2.h"
#include "tim2.h"
#include "tim3.h"
#include "tim5.h"
#include "usart2.h"
#include "usart1.h"
#include "pid.h"
#include "queue.h"
#include "macro.h"
#include "canctrollist.h"
#include "dianji.h"
#include "led8.h"
#include "vesc.h"
#include "DataScope_DP.h"
//START任务
#define START_TASK_PRIO 5
#define START_STK_SIZE 256
__align(8) OS_STK START_TASK_STK[START_STK_SIZE];
static void Task_Start(void *pdata);

//DJ任务
#define MOTOR_TASK_PRIO 7
#define MOTOR_STK_SIZE 256
__align(8) OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
static void Task_Motor(void *pdata);

//VESC任务
#define VESC_TASK_PRIO 16
#define VESC_STK_SIZE 256
__align(8)  OS_STK VESC_TASK_STK[VESC_STK_SIZE];
static void Task_VESC(void *pdata);

//虚拟示波器
#define DataSCOPE_TASK_PRIO 18
#define DataSCOPE_STK_SIZE 128
__align(8) OS_STK DataSCOPE_TASK_STK[DataSCOPE_STK_SIZE];
static void Task_DataScope(void *pdata);

#endif
