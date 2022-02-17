#ifndef _SPI2_H
#define _SPI2_H

#include "includes.h" //任何文件必须引入
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdbool.h"
#include "macro.h"
#include "delay.h"

#define CS0 GPIOB->BSRRL = GPIO_Pin_10
#define CS1 GPIOB->BSRRH = GPIO_Pin_10

/*******************寄存器地址********************/
//CAN控制 REQOP请求工作(7-5)、ABAT中止(4)、OSM单触发(3)、CLKEN CLKOUT引脚(2)、CLKOUT引脚预分频(0-1)
#define CANCTRL 0x0F
//CAN状态 (7-5工作模式)OPMOD、(3-1中断标志)ICOD
#define CANSTAT 0x0E //CAN状态寄存器
#define CNF1 0x2A    //配置寄存器1(bit 7-6 SJW)(bit 5-0 BRP波特率预分频比位)
#define CNF2 0x29    //配置寄存器2(bit7 BTLMODE相位缓冲段 PS2 位时间长度位)、(bit 6 SAM：采样点配置位)、(bit 5-3 PHSEG1 相位缓冲段 PS1 位时间长度位)、(bit 2-0 PRSEG：传播段长度位)
#define CNF3 0x28    //配置寄存器3(bit 7 SOF：起始帧信号位)、(bit 2-0 PHSEG2：相位缓冲段 PS2 长度)
#define BFPCTRL 0x0C //接收控制和状态寄存器

//RXMn (STANDARD or EXTANDARD) ID (HIGH or LOW)
//验收屏蔽寄存器
#define RXM0SIDH 0x20 //标准标识符
#define RXM0SIDL 0x21
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM0EID8 0x22 //扩展标识符
#define RXM0EID0 0x23
#define RXM1EID8 0x26
#define RXM1EID0 0x27

//RXFn (STANDARD or EXTANDARD) ID (HIGH or LOW)
//验收滤波寄存器
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B

//RXBnCTRL (CONTROL)
//接收缓冲器控制寄存器
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70
//RXBnSID
//接收缓冲器标准标识符
#define RXB0SIDH 0x61
#define RXB1SIDH 0x71
#define RXB0SIDL 0x62
#define RXB1SIDL 0x72
//RXBnEID
//接收缓冲器扩展标识符
#define RXB0EID8 0x63
#define RXB1EID8 0x73
#define RXB0EID0 0x64
#define RXB1EID0 0x74
//RXBnDLC
//接收缓冲器数据长度码
#define RXB0DLC 0x65
#define RXB1DLC 0x75

//中断标志寄存器
#define CANINTF 0x2C //MERRF WAKIF ERRIF TX2IF TX1IF TX0IF RX1IF RX0IF
//中断使能寄存器
#define CANINTE 0x2B //MERRE WAKIE ERRIE TX2IE TX1IE TX0IE RX1IE RX0IE
//报文错误 唤醒 错误 发送2空 发送1空 发送0空 接收1满 接收0满

/*******************输入指令集********************/
// CAN SPI commands
#define CAN_RESET 0xC0
#define CAN_READ 0x03
#define CAN_WRITE 0x02

//请求工作模式
#define REQOP_CONFIG 0x80
#define REQOP_LISTEN 0x60
#define REQOP_LOOPBACK 0x40
#define REQOP_SLEEP 0x20
#define REQOP_NORMAL 0x00
//时钟使、失能
#define CLKOUT_ENABLED 0x04
#define CLKOUT_DISABLED 0x00
//时钟分频
#define CLKOUT_PRE_8 0x03
#define CLKOUT_PRE_4 0x02
#define CLKOUT_PRE_2 0x01
#define CLKOUT_PRE_1 0x00

/* MCP2515 CAN  报文数据结构 */
typedef struct
{
    u32 StdId;  /* 标准帧 ID */
    u32 ExtId;  /* 拓展帧 ID */
    u8 IDE;     /* 0 : 标准帧  1：拓展帧 */
    u8 RTR;     /* 0 ：数据帧  1：远程帧 */
    u8 DLC;     /* 数据长度 */
    u8 Data[8]; /* 数据内容 */
} MCP2515_MSG;

extern bool MCP_Receive;
u32 MCP_READ(u32 addr);
u8 SPI2_ReadWriteByte(u8 TxData);
void MCP_RESET(void);
void MCP_WRITE(u8 addr, u8 message);

void MCP2515_Init(void);
void SPI2_Init(void);

#endif
