#ifndef _PARAM_H_
#define _PARAM_H_

#include "stm32f4xx.h"
#include "includes.h"
#include "stdbool.h"
#include "elmo.h"
#include "led.h"
#include "led8.h"
#include "filter.h"
#include "vesc.h"
#define RESET_PRO       \
  {                     \
    __set_FAULTMASK(1); \
    NVIC_SystemReset(); \
  }
//��һλ
#define SYSTEMERROR 0x0000
#define DJERROR 		0x0001
#define ELMOERROR 	0x0002
#define VESCERROR 	0x0003
#define EPOSERROR 	0x0004

//����λ
#define TIMEOUT 		0x0100
#define STUCK 			0x0200
#define EMERGENCY 	0x0300

typedef enum
{
  USART1_Tx_BufferSize = 256, //����һ�������ݳ���
  USART2_Tx_BufferSize = 256, //���ڶ��������ݳ���
  USART3_Tx_BufferSize = 32,  //�������������ݳ���
  USART1_Rx_BufferSize = 32,  //����һ�������ݳ��ȣ������ж���ʹ�ã�
  USART2_Rx_BufferSize = 32,  //���ڶ��������ݳ��ȣ������ж���ʹ�ã�
  USART3_Rx_BufferSize = 32,  //�������������ݳ��ȣ������ж���ʹ�ã�
} Usart_Enum;


/****��־λ�ṹ��****/
typedef struct{
    bool valve[8];                     //��ǰ��ŷ�״̬
    volatile bool Can1CtrlList_Enable; // CAN1���Ŀ��Ʊ�������־λ
    volatile bool Can2CtrlList_Enable; // CAN2���Ŀ��Ʊ�������־λ
    volatile bool Can1SendqueueFULL;   // CAN1���Ͷ�����
    volatile bool Can2SendqueueFULL;   // CAN2���Ͷ�����
		volatile bool VESCSendqueueFULL;   //VESC���Ͷ�����	
    volatile bool CanSendqueueEMPTY;   // CAN����Ϊ��
		volatile bool Usart1DMASendFinish; // Usart1_DMA����
		volatile bool Usart2DMASendFinish; // Usart2_DMA����
		volatile bool Usart3DMASendFinish; // Usart2_DMA����
		volatile bool MottorErrorFlag[4];
} FlagTypeDef;

typedef struct
{
	bool set_all_begin;
	bool set_all_enable;
	vs16 set_all_speed;
	vs16 set_all_angle;
	vs16 set_all_speedlimit;
}SET_ALL_TypeDef;

typedef struct
{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //����һ��������
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //����һ��������

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //���ڶ���������
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //���ڶ���������

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //��������������
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //��������������
} Usart_Struct;


/****������ʾ����****/
typedef struct _ErrorList
{
	u16 code;
	struct _ErrorList *next;
}ErrorList;

typedef struct
{
	u8 size;
	ErrorList *head;
}ErrorTypeDef;


/****�ṹ�嶨��****/
extern FlagTypeDef flag;
extern Usart_Struct usart;
extern ErrorTypeDef Error;

void ChangeData(u8 *buff2, u8 *buff1);
void Flag_Init(void);
void param_Init(void);
ErrorList *createError(void);
ErrorList *ErrorFind(ErrorList *list, uc16 errorcode);
void deletError(ErrorList *list, ErrorList *listdel);
void insertError(ErrorList *list, uc16 errorcode );


#endif
