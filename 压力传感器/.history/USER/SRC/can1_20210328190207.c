#include "delay.h"

/** 
  * @brief  延时、毫秒
  */
	
void Delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/** 
  * @brief  延时、微秒
  */
void Delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/** 
  * @brief  延时、指令周期数
  */

void Delay(u16 t)
{
	while(t--);
}
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
		/* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;  											//非时间触发通道模式
  CAN_InitStructure.CAN_ABOM=ENABLE;  											//软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM=DISABLE;  											//眠模式由软件唤醒
  CAN_InitStructure.CAN_NART=DISABLE;  											//DISABLE打开报文自动发送，错误时会重发
  CAN_InitStructure.CAN_RFLM=DISABLE;  											//报文不锁定，新的覆盖旧的
  CAN_InitStructure.CAN_TXFP=DISABLE;  											//发送FIFO的优先级由标识符决定
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;								//CAN硬件工作在正常模式

	  /* Seting BaudRate */
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;										//重新同步跳跃宽度为一个时间单位
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 										//时间段1占用8个时间单位
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;										//时间段2占用7个时间单位
	CAN_InitStructure.CAN_Prescaler=3; 												//分频系数（Fdiv）
	CAN_Init(CAN1, &CAN_InitStructure); 											//初始化CAN1
	/* 波特率计算公式: BaudRate = APB1时钟频率/Fdiv/（SJW+BS1+BS2） */
	/* 42MHz/3/(1+9+4)=1Mhz */
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =(uint16_t)((uint32_t)(0x00004170<<3)>>16);       //0000 0101 0001 0100 0001 0011 0		1		1		1 1111
	CAN_FilterInitStructure.CAN_FilterIdLow =(uint16_t)((uint32_t)(0x00004170<<3)>>0);				 //																(Emer)(Data)
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(uint16_t)((uint32_t)(0x00004171<<3)>>16);  //0001 1111 1111 1111 1111 1111 1		0		0		1 0000
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(uint16_t)((uint32_t)(0x00004171<<3)>>0);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//列表模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =(uint16_t)((uint32_t)(0x00004172<<3)>>16);
	CAN_FilterInitStructure.CAN_FilterIdLow =(uint16_t)((uint32_t)(0x00004172<<3)>>0);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(uint16_t)((uint32_t)(0xFFF0FFFF<3)>>16);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(uint16_t)((uint32_t)(0xFFF0FFFF<<3)>>0);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_ERR|CAN_IT_LEC, ENABLE);

}

void CAN1_RX0_IRQHandler()
{
	CanRxMsg rx_message;
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		if ((rx_message.ExtId >= 0x00004170) && (rx_message.ExtId < 0x00004173))
		{
			if ((rx_message.Data[0] == 'G') && (rx_message.Data[1] == 'E') && (rx_message.Data[2] == 'T'))
			{
				answer_HX711_feedback(&rx_message);
			}

			if ((rx_message.Data[0] == 'p') && (rx_message.Data[1] == 'r') && (rx_message.Data[2] == 'e'))
			{
				Can1_Send_Pressure();
			}
		}
	}
}


void Can1_Send_Pressure(void)
{
	CanTxMsg txMessage;
	txMessage.ExtId = ID_BACK;
	txMessage.RTR = CAN_RTR_Data;	 //消息类型为数据帧，一帧8位
	txMessage.IDE = CAN_Id_Extended; //使用扩展标识符
	txMessage.DLC = 5;
	txMessage.Data[0] = 'p';
	txMessage.Data[1] = 'r';
	txMessage.Data[2] = 'e';
	txMessage.Data[3] = Pressure & 0xff;
	txMessage.Data[4] = (Pressure >> 8) & 0xff;

	CAN_Transmit(CAN1, &txMessage);
}

void answer_HX711_feedback(const CanRxMsg *rxmessage)
{
	CanTxMsg txmessage;
	txmessage.ExtId = ID_BACK;
	txmessage.RTR = CAN_RTR_Data;
	txmessage.IDE = CAN_Id_Extended;
	txmessage.DLC = rxmessage->DLC;
	txmessage.Data[0] = (rxmessage->Data[0]) + 32;
	txmessage.Data[1] = (rxmessage->Data[1]) + 32;
	txmessage.Data[2] = (rxmessage->Data[2]) + 32;
	txmessage.Data[3] = rxmessage->Data[3];
	txmessage.Data[4] = rxmessage->Data[4];
	txmessage.Data[5] = rxmessage->Data[5];
	txmessage.Data[6] = rxmessage->Data[6];
	txmessage.Data[7] = rxmessage->Data[7];
	
		CAN_Transmit(CAN1, &txmessage);

}
