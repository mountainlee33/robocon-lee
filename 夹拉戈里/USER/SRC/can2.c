#include "can2.h"
extern DJmotor motor[8];
u32 last_update_time[8] = {0};
u32 now_update_time[8] = {0};
u32 err_update_time[8] = {0};

void CAN2_Mode_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);//GPIO_PinAFConfig端口复用功能
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

	/* Configure CAN pin: RX  TX*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* CAN RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ中断使能
	NVIC_Init(&NVIC_InitStructure);

	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
	CAN_InitStructure.CAN_ABOM = ENABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
	CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
	CAN_InitStructure.CAN_NART = DISABLE; //禁止报文自动发送，即只发送一次，无论结果如何
	CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
	CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	  //CAN硬件工作在正常模式

	/* Seting BaudRate */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	 //重新同步跳跃宽度为一个时间单位
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;	 //时间段1占用8个时间单位
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;	 //时间段2占用7个时间单位
	CAN_InitStructure.CAN_Prescaler = 3; //分频系数（Fdiv）
	CAN_Init(CAN2, &CAN_InitStructure);	 //初始化CAN2
	
	/* 波特率计算公式: BaudRate = APB1时钟频率/Fdiv/（SJW+BS1+BS2） */ 
	/* 42MHz/3/(1+9+4)=1Mhz */
	
	//DJ
	CAN_FilterInitStructure.CAN_FilterNumber = 14;//列表模式且16位信息  存四个id
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0X201 << 5;//CAN_FilterIdHigh包含的是STD[0~10]和EXID[13~17]
	CAN_FilterInitStructure.CAN_FilterIdLow = 0X202 << 5;//标准CAN ID本身是不包含扩展ID数据，
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X203 << 5;//因此为了要将标准CAN ID放入此寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X204 << 5;//标准CAN ID首先应左移5位后才能对齐
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 15;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0X205 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0X206 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X207 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X208 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	
}


	void CAN2_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
		
		if ((rx_message.StdId >= 0x201) && (rx_message.StdId <= 0x208) && (rx_message.RTR == CAN_RTR_Data)) //DJ电机报文处理
		{
			u8 id = rx_message.StdId - 0x201;
			last_update_time[id] = now_update_time[id];
			now_update_time[id] = OSTimeGet();
			err_update_time[id] = now_update_time[id] - last_update_time[id];
			motor[id].valueReal.velocity = (rx_message.Data[2] << 8) | (rx_message.Data[3]);
			motor[id].valueReal.pulseread = (rx_message.Data[0] << 8) | (rx_message.Data[1]);
			motor[id].valueReal.current = (rx_message.Data[4] << 8) | (rx_message.Data[5]);
			motor[id].valueReal.temperature = rx_message.Data[6];
			pulse_caculate(id);
			if (!motor[id].status.clearFlag) //上电第一次进中断清除位置计算误差。
			{
			motor[id].status.clearFlag = true;
			motor[id].argum.distance = 0;
			motor[id].valueReal.pulse = 0;
			}
		}
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
	CAN_ClearFlag(CAN2, CAN_IT_FMP0);
	}
}

void CAN2_RX1_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET)
	{
		CanRxMsg rx_message;
 		CAN_Receive(CAN2,CAN_FIFO1,&rx_message);
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
		CAN_ClearFlag(CAN2,CAN_IT_FMP1);
	}
}
//发送报文
u8 CAN2_Send_Msg(u8 *msg, u8 len)
{
	u16 i=0;
	u8 mbox;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x12; // 标准标识符为 0
	TxMessage.ExtId=0x12; // 设置扩展标示符（29 位）
	TxMessage.IDE=0; // 使用扩展标识符
	TxMessage.RTR=0; // 消息类型为数据帧，一帧 8 位
	TxMessage.DLC=len; // 发送两帧信息
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i]; // 第一帧信息 
	mbox= CAN_Transmit(CAN2, &TxMessage); 
	i=0;
	while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;
	if(i>=0XFFF)return 1;//发送
	
	return 0;
}

/****接收到报文反馈后控制表处理****/
//static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
//{
//	Can_MesgSentList->ReceiveNumber+=1;
//	Can_MesgSentList->TimeOut=0;
//	Can_MesgSentList->SendSem--;
//	Can_MesgSentList->SentQueue.Front=(Can_MesgSentList->SentQueue.Front+1)%CAN_QUEUESIZE;
//}

//接收报文
u8 CAN2_Receive_Msg(u8 *buf)
{ 
	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;//没有接收到数据,直接退出
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据
	for(i=0;i<RxMessage.DLC;i++)
	buf[i]=RxMessage.Data[i]; 
	return RxMessage.DLC; //返回接受到的数据长度
}
