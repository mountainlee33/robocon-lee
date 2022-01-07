#include "can1.h"
//1 个 32 位过滤器，包括：STDID[10:0]、EXTID[17:0]、IDE 和 RTR 位
//2 个 16 位过滤器，包括：STDID[10:0]、IDE、RTR 和 EXTID[17:15]位
//扩展帧使用32位，标准帧使用16位
//CAN_BTR寄存器，设置分频、Tbs1(1~16)、Tbs2(1~8)以及Tsjw(1~3)
//CAN_TDLxR、CAN_TDHxR用来存储8个字节
u8 CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{
    u16 i = 0;
    if (tsjw == 0 || tbs2 == 0 || tbs1 == 0 || brp == 0)
        return 1;

    RCC->AHB1ENR = 1 << 0;
    GPIO_Set(GPIOA, PIN11 | PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP,
             GPIO_SPEED_50M, GPIO_PUPD_PU); //PA11,PA12,复用功能，上拉输出
    //GPIO_AF_Set函数调用复用功能，复用功能看使用io口
    GPIO_AF_Set(GPIOA, 11, 9); //AF9为can的复用
    RCC->APB1ENR = 1 << 25;    //使能CAN1时钟，使用的是APB1的时钟
    CAN1->MCR = 0x0000;        //退出睡眠模式(同时设置所有位为0)
    CAN1->MCR = 1 << 0;        //请求CAN进入初始化模式
    while ((CAN1->MSR & 1 << 0) == 0)
    {
        i++;
        if (i > 100)
            return 2; //进入初始化模式失败
    }
    CAN1->MCR |= 0 << 7;     //非时间出发通信模式
    CAN1->MCR |= 0 << 6;     //软件自动离线管理
    CAN1->MCR |= 0 << 5;     //睡眠模式通过软件唤醒(清除 CAN1->MCR 的 SLEEP 位)
    CAN1->MCR |= 1 << 4;     //禁止报文自动传送
    CAN1->MCR |= 0 << 3;     //报文不锁定,新的覆盖旧的
    CAN1->MCR |= 0 << 2;     //优先级由报文标识符决定
    CAN1->BTR = 0x00000000;  //清除原来的设置.
    CAN1->BTR |= mode << 30; //模式设置 0,普通模式;1,回环模式;
    CAN1->BTR |= tsjw << 24; //重新同步跳跃宽度(Tsjw)为 tsjw+1 个时间单位
    CAN1->BTR |= tbs2 << 20; //Tbs2=tbs2+1 个时间单位
    CAN1->BTR |= tbs1 << 16; //Tbs1=tbs1+1 个时间单位
    CAN1->BTR |= brp << 0;   //分频系数(Fdiv)为 brp+1
    //波特率:Fpclk1/((Tbs1+Tbs2+1)*Fdiv)
    CAN1->MCR &= ~(1 << 0); //请求 CAN 退出初始化模式
    while ((CAN1->MSR & 1 << 0) == 1)
    {
        i++;
        if (i > 0xFFF0)
            return 3; //退出初始化模式失败
    }
    //过滤器初始化
    CAN1->FMR |= 1 << 0;
}