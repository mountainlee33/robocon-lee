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

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);//GPIO_PinAFConfig�˿ڸ��ù���
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);

	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE; //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM = ENABLE; //�����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
	CAN_InitStructure.CAN_AWUM = DISABLE; //˯��ģʽ���������
	CAN_InitStructure.CAN_NART = DISABLE; //��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
	CAN_InitStructure.CAN_RFLM = DISABLE; //���Ĳ��������µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP = DISABLE; //����FIFO�����ȼ��ɱ�ʶ������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	  //CANӲ������������ģʽ

	/* Seting BaudRate */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	 //����ͬ����Ծ���Ϊһ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;	 //ʱ���1ռ��8��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;	 //ʱ���2ռ��7��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler = 3; //��Ƶϵ����Fdiv��
	CAN_Init(CAN2, &CAN_InitStructure);	 //��ʼ��CAN2
	
	/* �����ʼ��㹫ʽ: BaudRate = APB1ʱ��Ƶ��/Fdiv/��SJW+BS1+BS2�� */ 
	/* 42MHz/3/(1+9+4)=1Mhz */
	
	//DJ
	CAN_FilterInitStructure.CAN_FilterNumber = 14;//�б�ģʽ��16λ��Ϣ  ���ĸ�id
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0X201 << 5;//CAN_FilterIdHigh��������STD[0~10]��EXID[13~17]
	CAN_FilterInitStructure.CAN_FilterIdLow = 0X202 << 5;//��׼CAN ID�����ǲ�������չID���ݣ�
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X203 << 5;//���Ϊ��Ҫ����׼CAN ID����˼Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X204 << 5;//��׼CAN ID����Ӧ����5λ����ܶ���
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
		
		if ((rx_message.StdId >= 0x201) && (rx_message.StdId <= 0x208) && (rx_message.RTR == CAN_RTR_Data)) //DJ������Ĵ���
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
			if (!motor[id].status.clearFlag) //�ϵ��һ�ν��ж����λ�ü�����
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
//���ͱ���
u8 CAN2_Send_Msg(u8 *msg, u8 len)
{
	u16 i=0;
	u8 mbox;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x12; // ��׼��ʶ��Ϊ 0
	TxMessage.ExtId=0x12; // ������չ��ʾ����29 λ��
	TxMessage.IDE=0; // ʹ����չ��ʶ��
	TxMessage.RTR=0; // ��Ϣ����Ϊ����֡��һ֡ 8 λ
	TxMessage.DLC=len; // ������֡��Ϣ
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i]; // ��һ֡��Ϣ 
	mbox= CAN_Transmit(CAN2, &TxMessage); 
	i=0;
	while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;
	if(i>=0XFFF)return 1;//����
	
	return 0;
}

/****���յ����ķ�������Ʊ���****/
//static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
//{
//	Can_MesgSentList->ReceiveNumber+=1;
//	Can_MesgSentList->TimeOut=0;
//	Can_MesgSentList->SendSem--;
//	Can_MesgSentList->SentQueue.Front=(Can_MesgSentList->SentQueue.Front+1)%CAN_QUEUESIZE;
//}

//���ձ���
u8 CAN2_Receive_Msg(u8 *buf)
{ 
	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;//û�н��յ�����,ֱ���˳�
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//��ȡ����
	for(i=0;i<RxMessage.DLC;i++)
	buf[i]=RxMessage.Data[i]; 
	return RxMessage.DLC; //���ؽ��ܵ������ݳ���
}
