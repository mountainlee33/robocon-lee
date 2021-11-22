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

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
	CAN_Init(CAN2, &CAN_InitStructure);	 //初始化CAN1

	/* 波特率计算公式: BaudRate = APB1时钟频率/Fdiv/（SJW+BS1+BS2） */
	/* 42MHz/3/(1+9+4)=1Mhz */

	//配置过滤器
	//DJ
	CAN_FilterInitStructure.CAN_FilterNumber = 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0X201 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0X202 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X203 << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X204 << 5;
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
	//VESC
	CAN_FilterInitStructure.CAN_FilterNumber = 20;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((((u32)CAN_PACKET_STATUS <<8)<<3) & 0xffff0000) >> 16;
	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)CAN_PACKET_STATUS <<8)<<3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xffffff00 << 3) >> 16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xffffff00 << 3) & 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
  CAN_FilterInitStructure.CAN_FilterNumber = 25;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((((uint32_t)CAN_PACKET_STATUS_4 << 8) << 3) & 0xffff0000) >> 16;
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)CAN_PACKET_STATUS_4 << 8) << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xffffff00 << 3) >> 16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xffffff00 << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
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
		
#ifdef USE_VESC
		if((rx_message.IDE == CAN_ID_EXT) && (rx_message.RTR == CAN_RTR_DATA))
		{
			int32_t ind = 0 ;
			u8 id = (rx_message.ExtId & 0x0f) - 1;
			if((rx_message.ExtId >> 8 ) ==CAN_PACKET_STATUS)
			{
				VESCmotor[id].ValReal.speed = get_s32_from_buffer(rx_message.Data, &ind) / VESCmotor[id].instrinsic.POLE_PAIRS;
				VESCmotor[id].ValReal.current = buffer_16_to_float(rx_message.Data , 1e1, &ind);
			}
			else if ((rx_message.ExtId >> 8) == CAN_PACKET_STATUS_4)
			{
				VESCmotor[id].ValReal.angle = buffer_16_to_float(rx_message.Data, 5e1 , &ind);				
				VESCmotor[id].ValReal.angle = buffer_16_to_float(rx_message.Data, 5e1 , &ind);	
				VESCmotor[id].ValReal.angle = buffer_16_to_float(rx_message.Data, 5e1 , &ind);	
				VESCmotor[id].ValReal.angle = buffer_16_to_float(rx_message.Data, 5e1 , &ind);	
			}
				//位置计算
				ChangeData(&rx_message.Data[6],&rx_message.Data[7]);
				DecodeU16Data(&VESCmotor[id].argum.angleNow,&rx_message.Data[6]);
				VESCmotor[id].argum.distance = VESCmotor[id].argum.angleNow - VESCmotor[id].argum.anglePrv;
				if(VESCmotor[id].argum.firstPos)//清除起始误差
				{
					VESCmotor[id].argum.firstPos = false;
					VESCmotor[id].argum.distance = 0;
				}
				VESCmotor[id].argum.anglePrv = VESCmotor[id].argum.angleNow;
				if(ABS(VESCmotor[id].argum.distance) > 9000)
					VESCmotor[id].argum.distance -= SIG(VESCmotor[id].argum.distance) * 18000;
				VESCmotor[id].ValReal.position += VESCmotor[id].argum.distance/5;				
				//位置残差更新
				VESCmotor[id].argum.difPosition = VESCmotor[id].ValSet.position - VESCmotor[id].ValReal.position;
				//锁点记录
				if(VESCmotor[id].begin)
				{
					VESCmotor[id].argum.lockAngle = VESCmotor[id].ValReal.angle;
					VESCmotor[id].argum.lockPosition = VESCmotor[id].ValReal.position;
				}
			
			VESCmotor[id].argum.lastRxTim = OSTimeGet();
		 }
	 }
}
#endif

