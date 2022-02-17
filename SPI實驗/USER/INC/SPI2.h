#ifndef _SPI2_H
#define _SPI2_H

#include "includes.h" //�κ��ļ���������
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdbool.h"
#include "macro.h"
#include "delay.h"

#define CS0 GPIOB->BSRRL = GPIO_Pin_10
#define CS1 GPIOB->BSRRH = GPIO_Pin_10

/*******************�Ĵ�����ַ********************/
//CAN���� REQOP������(7-5)��ABAT��ֹ(4)��OSM������(3)��CLKEN CLKOUT����(2)��CLKOUT����Ԥ��Ƶ(0-1)
#define CANCTRL 0x0F
//CAN״̬ (7-5����ģʽ)OPMOD��(3-1�жϱ�־)ICOD
#define CANSTAT 0x0E //CAN״̬�Ĵ���
#define CNF1 0x2A    //���üĴ���1(bit 7-6 SJW)(bit 5-0 BRP������Ԥ��Ƶ��λ)
#define CNF2 0x29    //���üĴ���2(bit7 BTLMODE��λ����� PS2 λʱ�䳤��λ)��(bit 6 SAM������������λ)��(bit 5-3 PHSEG1 ��λ����� PS1 λʱ�䳤��λ)��(bit 2-0 PRSEG�������γ���λ)
#define CNF3 0x28    //���üĴ���3(bit 7 SOF����ʼ֡�ź�λ)��(bit 2-0 PHSEG2����λ����� PS2 ����)
#define BFPCTRL 0x0C //���տ��ƺ�״̬�Ĵ���

//RXMn (STANDARD or EXTANDARD) ID (HIGH or LOW)
//�������μĴ���
#define RXM0SIDH 0x20 //��׼��ʶ��
#define RXM0SIDL 0x21
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM0EID8 0x22 //��չ��ʶ��
#define RXM0EID0 0x23
#define RXM1EID8 0x26
#define RXM1EID0 0x27

//RXFn (STANDARD or EXTANDARD) ID (HIGH or LOW)
//�����˲��Ĵ���
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
//���ջ��������ƼĴ���
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70
//RXBnSID
//���ջ�������׼��ʶ��
#define RXB0SIDH 0x61
#define RXB1SIDH 0x71
#define RXB0SIDL 0x62
#define RXB1SIDL 0x72
//RXBnEID
//���ջ�������չ��ʶ��
#define RXB0EID8 0x63
#define RXB1EID8 0x73
#define RXB0EID0 0x64
#define RXB1EID0 0x74
//RXBnDLC
//���ջ��������ݳ�����
#define RXB0DLC 0x65
#define RXB1DLC 0x75

//�жϱ�־�Ĵ���
#define CANINTF 0x2C //MERRF WAKIF ERRIF TX2IF TX1IF TX0IF RX1IF RX0IF
//�ж�ʹ�ܼĴ���
#define CANINTE 0x2B //MERRE WAKIE ERRIE TX2IE TX1IE TX0IE RX1IE RX0IE
//���Ĵ��� ���� ���� ����2�� ����1�� ����0�� ����1�� ����0��

/*******************����ָ�********************/
// CAN SPI commands
#define CAN_RESET 0xC0
#define CAN_READ 0x03
#define CAN_WRITE 0x02

//������ģʽ
#define REQOP_CONFIG 0x80
#define REQOP_LISTEN 0x60
#define REQOP_LOOPBACK 0x40
#define REQOP_SLEEP 0x20
#define REQOP_NORMAL 0x00
//ʱ��ʹ��ʧ��
#define CLKOUT_ENABLED 0x04
#define CLKOUT_DISABLED 0x00
//ʱ�ӷ�Ƶ
#define CLKOUT_PRE_8 0x03
#define CLKOUT_PRE_4 0x02
#define CLKOUT_PRE_2 0x01
#define CLKOUT_PRE_1 0x00

/* MCP2515 CAN  �������ݽṹ */
typedef struct
{
    u32 StdId;  /* ��׼֡ ID */
    u32 ExtId;  /* ��չ֡ ID */
    u8 IDE;     /* 0 : ��׼֡  1����չ֡ */
    u8 RTR;     /* 0 ������֡  1��Զ��֡ */
    u8 DLC;     /* ���ݳ��� */
    u8 Data[8]; /* �������� */
} MCP2515_MSG;

extern bool MCP_Receive;
u32 MCP_READ(u32 addr);
u8 SPI2_ReadWriteByte(u8 TxData);
void MCP_RESET(void);
void MCP_WRITE(u8 addr, u8 message);

void MCP2515_Init(void);
void SPI2_Init(void);

#endif
