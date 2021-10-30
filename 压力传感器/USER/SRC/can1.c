/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-04-04 21:54:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\SRC\can1.c
 */
#include "can1.h"

void CAN1_Configuration(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	
	  /* Configure CAN pin: RX A11  TX A12 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	/* CAN RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* CAN BOF interrupt */
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_SCE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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
	u8 num = 0 ;	
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

			if ((rx_message.Data[0] == 'P') && (rx_message.Data[1] == 'R')&&(rx_message.ExtId == ID_SELF))
			{
				//接收到主控信号后，反馈自检信号，并使能标志位，TIM3发送数据
				transfer_enable =1;
				answer_HX711_feedback(&rx_message);	
				
			//判断是否某个传感器出现错误，并将错误信息反馈给主控
				for(num=0;num<SensorNum;num++)
				{
					if (presensor[num].preflag.DOUT_error == 1)
					{
						HX711_error(&presensor[num]);
					}
				}
			}
	}
}

void Can1_Send_Pressure(void)
{
	CanTxMsg TXmessage;
	TXmessage.ExtId = ID_BACK;
	TXmessage.RTR = CAN_RTR_Data;
	TXmessage.IDE = CAN_Id_Extended;
	TXmessage.DLC = 8;
	TXmessage.Data[0] = 'p';
	TXmessage.Data[1] = 'r';
	TXmessage.Data[2] = (uint8_t)((int16_t)presensor[0].pressure) ;
	TXmessage.Data[3] = (uint8_t)(((int16_t)presensor[0].pressure)>>8) ;
	TXmessage.Data[4] = (uint8_t)((int16_t)presensor[1].pressure) ;
	TXmessage.Data[5] = (uint8_t)(((int16_t)presensor[1].pressure)>>8) ;
	TXmessage.Data[6] = (uint8_t)((int16_t)presensor[2].pressure) ;
	TXmessage.Data[7] = (uint8_t)(((int16_t)presensor[2].pressure)>>8) ;
	
	CAN_Transmit(CAN1,&TXmessage);
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

void HX711_error(PRESENSOR *Presensor)
{
	CanTxMsg txmsg;
	txmsg.ExtId = ID_BACK;
	txmsg.RTR = CAN_RTR_Data;
	txmsg.IDE = CAN_Id_Extended;
	txmsg.DLC = 5;
	txmsg.Data[0]='p';
	txmsg.Data[1]='r';
	txmsg.Data[2]=Presensor->constpre.number;
	txmsg.Data[3]='e';
	txmsg.Data[4]='r';
}