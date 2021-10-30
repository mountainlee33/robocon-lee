#include "can1.h"

void CAN1_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef Can_FilterInitStrucutre;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
   
	GPIO_Set(GPIOA,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_25M,GPIO_PUPD_NONE);
    GPIO_AF_Set(GPIOA,11,9);    //CAN1_RX PA11        
    GPIO_AF_Set(GPIOA,12,9);	//CAN1_TX PA12

//GPIO复用设置GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
//BITx:0~15,代表IO引脚编号；AF9：CAN1/CAN2/TIM12~14
  
	/* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
    CAN_InitStructure.CAN_ABOM = DISABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
    CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
    CAN_InitStructure.CAN_NART = DISABLE; //禁止报文自动发送，即只发送一次，无论结果如何
    CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
    CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN硬件工作在正常模式

    /* Seting BaudRate */
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
    CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
    CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1
	//波特率：42M/((9+1+4)*3)=1Mbps

    /* CAN filter Init */
	Can_FilterInitStrucutre.CAN_FilterNumber=0;//主控
	Can_FilterInitStrucutre.CAN_FilterMode=CAN_FilterMode_IdMask;
	Can_FilterInitStrucutre.CAN_FilterScale=CAN_FilterScale_32bit;
	Can_FilterInitStrucutre.CAN_FilterIdHigh=((ID_SELF<<3)&0xffff0000)>>16;
	Can_FilterInitStrucutre.CAN_FilterIdLow=(ID_SELF<<3)&0xffff;
	Can_FilterInitStrucutre.CAN_FilterMaskIdHigh=0x00;//(0xffff00<<3)>>16;
	Can_FilterInitStrucutre.CAN_FilterMaskIdLow=0x00;//(0xffff00<<3)&0xffff;
    Can_FilterInitStrucutre.CAN_FilterActivation=ENABLE;				
	Can_FilterInitStrucutre.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInit(&Can_FilterInitStrucutre);

    MY_NVIC_Init(1,1,CAN1_RX0_IRQn,3);//配置中断优先级等参数
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        answer_master(&rx_message);
    }
}

//主控反馈指令,检验收/发功能
static void answer_master(const CanRxMsg *rx_message)
{	
	CanTxMsg tx_message;
	tx_message.ExtId = ID_BACK;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = rx_message->DLC;
	tx_message.Data[0] = rx_message->Data[0]+0x40;
	tx_message.Data[1] = rx_message->Data[1];
	tx_message.Data[2] = rx_message->Data[2];
	tx_message.Data[3] = rx_message->Data[3];
	tx_message.Data[4] = rx_message->Data[4];
	tx_message.Data[5] = rx_message->Data[5];
	tx_message.Data[6] = rx_message->Data[6];
	tx_message.Data[7] = rx_message->Data[7];
	CAN_Transmit(CAN1,&tx_message);	
}


void Can1_BusTest(void)
{
	
	CanTxMsg TxMessage;
	TxMessage.ExtId=0X00020101;
	TxMessage.RTR=CAN_RTR_Data;	//消息类型为数据帧，一帧8位
	TxMessage.IDE=CAN_Id_Extended;//使用扩展标识符
	TxMessage.DLC=3;
	TxMessage.Data[0]='W';
	TxMessage.Data[1]='H';
	TxMessage.Data[2]='U';
	
	CAN_Transmit(CAN1,&TxMessage);	
}

void Can1_Send_Pressure(void)
{
	CanTxMsg txMessage;
	TxMessage.ExtId=ID_BACK;
	TxMessage.RTR=CAN_RTR_Data;	//消息类型为数据帧，一帧8位
	TxMessage.IDE=CAN_Id_Extended;//使用扩展标识符
	TxMessage.DLC=6;
	TxMessage.Data[0]='P';
	TxMessage.Data[1]='r';
	TxMessage.Data[2]='e';
	TxMessage.Data[3]=PreSensor;
	TxMessage.Data[4]=Pressure & 0xff;
	TxMessage.Data[5]=(Pressure>>8) & 0xff;

CAN_Transmit(CAN1，&txMessage);
}