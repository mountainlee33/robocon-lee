任务延时函数(10ms一次时钟节拍中断)：OSTimeDly()，最多延时65535个节拍
建立任務，OSTaskCreate(void(*task)(void *pd),void *pdata,OS_STK *ptos,int prio),task是指向任務代碼的指針，pdata是傳遞給任務參數的指針，ptos是分配給任務的堆棧的棧頂指針，prio是分配給任務的優先級
優先級必須各不相同，堆棧空間必須聲明為OS_STK類型

launch = OSSemCreate(0)建立一個信號量，可以作爲計數量，bool型變量
OSSemDel()刪除一個信號量，刪除信號量前必須刪除操作它的所有任務
OSSemPend()等待一個信號量，信號量非0，計數值遞減；0，則任務挂起
OSSemPost()發出一個信號量，查找是否有任務等待信號量，若沒有任務等待信號量，計數值加1

#include "includes.h"必加，通过这个头文件包含所有头文件
OSInit()创建空闲任务和统计任务
TaskStart()运行代码指针，初始化数据指针，堆栈栈顶，)任务
OSStart()先进行高优先级任务

软实时系统：不要求任务时间，使各个任务尽快运行
硬实时系统：执行无误且准时
前台：中断级，后台：任务级

临界段：进前关中断，进后开中断
任务状态：休眠态、就绪态、运行态、挂起态、中断态
www.uCOS-II.COM
任务切换所需时间取决于CPU有多少寄存器入栈

不可剥夺型内核：只有一个任务进行完，才能进行下一任务
可剥夺型内核：让高优先级任务先进行
小心优先级反转！！
任务间通信：通过全程变量，或发消息给另一个任务

使用共享数据结构，保证互斥条件：①关中断和开中断：OS_ENTER_CRITICAL()、OS_EXIT_CRITICAL()
信号量：①控制共享资源的使用权②标志某事件的发生③使2个任务行为同步

OS_ENTER_CRITICAL()禁止处理器响应中断请求
OS_EXIT_CRITICAL()打开处理器中断
OSTCBStkPtr指向当前任务堆栈栈顶的指针
OSTCBExtPtr指向用户定义的任务控制块扩展
OSTCBStkBottom指向任务堆栈栈底的指针
OSTCBMsg指向传递给任务的消息指针
OSTCBDly任务延时
OSTCBPrio任务优先级
OS_TCBInit()任务控制块初始化函数
OSSchcd()任务调度函数
OS_TASK_SW()任务切换函数
OSTaskIdle（）空闲任务
OS_TaskStat()统计任务
OSTimeDly()时间延迟函数

调用OSStart()之后，再开定时器中断

任务管理
建立任务，OSTaskCreate()、OSTaskCreateExt()
OSTaskCreate需要四个参数：task指向任务代码指针、pdata传递给任务的参数指针、ptos分配给任务的堆栈栈顶指针、prio分配给任务的优先级
其调用OSTaskStkInit()建立任务堆栈
任务堆栈定义需判断上出下进还是下出上进，可以同时定义#if.....#else....#endif
删除任务，OSTaskDel()①无法在中断服务中删除任务②无法删除空闲任务③可以删除自己
改变任务优先级，OSTaskChangePrio()
挂起任务，OSTaskSuspend()  调用OSTaskResume()来恢复挂起的任务

时间管理
任务延时函数，OSTimeDly()系统每隔10ms发生一次时钟节拍中断
按时、分、秒延时函数，OSTimeDlyHMSM()其中有时、分、秒、毫秒
系统时间，OSTimeGet()和OSTimeSet()

事件控制块ECB(Event Control Blocks)
OSEventType事件具体类型
OSEventPtr事件是消息邮箱或消息队列
OSEventTbl[]和OSEventGrp系统中就绪状态的任务
具体操作：①初始化事件控制块②使任务进入就绪态③使任务进入等待事件的状态④因为等待超时使任务进入就绪态
初始化事件控制块，OS_EventWaitListInit()
使一个任务进入就绪态，OS_EventTaskRdy()
使一个任务进入等待某事件发生状态，OS_EventTaskWait()

信号量管理
信号量：16位无符号整型计数值，等待信号量的任务组成的等待任务表
建立一个信号量，OSSemCreate()
删除一个信号量，OSSemDel()首先删除操作信号量的所有任务
等待一个信号量，OSSemPend()
发出一个信号量，OSSemPost()
无等待请求一个信号量，OSSemAccept()该信号量暂时无效，也可以让任务返回
查询一个信号量的当前状态，OSSemQuery()指向信号量对应控制块指针pevent,指向记录信号量信息的数据结构OS_SEM_DATA的指针pdata

互斥型信号量(mutex:mutual exclusion semaphores)管理
mutex：用于降解优先级反转问题
互斥型信号量组成：1个标志(指示mutex是否可以使用)、1个优先级(一旦高优先级任务需要mutex，赋给低优先级的)，1个等待的任务列表
建立一个互斥型信号量，OSMutexCreate()
删除一个互斥型信号量，OSMutexDel()当OS_MUTEX_DEL_EN置1时，才能有效
等待一个互斥型信号量(挂起)，OSMutexPend()
释放一个互斥型信号量，OSMutexPost()
无等待地获取互斥型信号量，OSMutexAccept()
获取互斥型信号量的当前状态，OSMutexQuery()

事件标志组管理
事件标志组(event flag)：①保存当前事件组中各事件状态的标志位②等待这些标志位或清除的任务列表
OS_FLAG_GRP事件标志组
OS_FLAG_NODE数据结构
建立一个事件标志组，OSFlagCreate()

ERROR:
程序死在了OSStart()里:
OSStartHang
B    OSStartHang