#include "usart2.h"
void USART2_Init()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	//使能串口、IO、DMA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	//复用串口PA2、PA3
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	//配置IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;				//波特率115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8字节
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //接受中断
	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//使能USART
	USART_Cmd(USART2, ENABLE);

	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							   //通道选择
	DMA_InitStructure.DMA_BufferSize = 0;									   //数据传输量
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					   //
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(usart.TxBuffer_USART2); //存储器0地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);		   //DMA外设地址
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					   //存储器增量模式
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		   //外设非增量模式
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			   //存储器数据长度：8位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	   //外设数据长度：8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							   //普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					   //以下为f4特有
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					   //FIFO模式禁止
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;		   //FIFO阈值
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		   //外设突发单次传输
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				   //存储器突发单次传输
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);								   //初始化stream6

	DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
	DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE); //关闭了DMA传输完成中断，导致数据无法传输。10/14/2018

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
}

void DMA1_Stream6_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
	if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
		flag.Usart2DMASendFinish = 0;
	}
}

//串口屏
//单片机接收数据处理信息
vu8 interface = 0;
u8 USART2_RX_STA = 0;
u8 ELMOID, DJID, VESCID;
void USART2_IRQHandler(void)
{
//	u8 signal = 0;
//	if (USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET) //溢出错误中断
//	{
//		USART_ReceiveData(USART2);
//		USART2_RX_STA = 0;
//		memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
	}
//	else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
//	{
//		USART_ClearFlag(USART2, USART_FLAG_RXNE);
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//		signal = USART_ReceiveData(USART2);
//		if ((USART2_RX_STA & 0x40) != 0)
//		{
//			if ((USART2_RX_STA & 0x80) == 0)
//			{
//				if (signal == 0xff)
//					USART2_RX_STA |= 0x80; //?
//				else
//				{
//					usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = signal;
//					USART2_RX_STA++;
//					if ((USART2_RX_STA & 0X3F) > 40)
//						USART2_RX_STA = 0;
//				}
//			}
//		}
//		else if (signal == 0xee)
//			USART2_RX_STA |= 0x40;
//		if ((USART2_RX_STA & 0x80) != 0) //串口屏指令判断
//		{
//			if (usart.RxBuffer_USART2[1] == 0x01)
//			{
//				interface = usart.RxBuffer_USART2[3];
//			}
//			switch (usart.RxBuffer_USART2[3])
//			{
//			case 0x00: //main界面
//				break;
//			case 0x01: //DJ界面
//			{
//				if ((usart.RxBuffer_USART2[5] == 0x08) && (usart.RxBuffer_USART2[6] == 0x11)) //ID号
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x30:
//						DJID = 0;
//						break;
//					case 0x31:
//						DJID = 1;
//						break;
//					case 0x32:
//						DJID = 2;
//						break;
//					case 0x33:
//						DJID = 3;
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x09) && (usart.RxBuffer_USART2[6] == 0x10)) //使能
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					motor[DJID].enable = 1;
//					break;
//					case 0x00:
//					motor[DJID].enable = 0;
//					break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x0A) && (usart.RxBuffer_USART2[6] == 0x10)) //开始
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					motor[DJID].begin = 1;
//					break;
//					case 0x00:
//					motor[DJID].begin = 0;
//					break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x03) && (usart.RxBuffer_USART2[6] == 0x11)) //模式
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x32:
//						motor[DJID].mode = 2;
//						break;
//					case 0x34:
//						motor[DJID].mode = 4;
//						break;
//					case 0x35:
//						motor[DJID].mode = 5;
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x01) && (usart.RxBuffer_USART2[6] == 0x11)) //转速设置
//				{
//					motor[DJID].valueSet.velocity = atof((char *)(&usart.RxBuffer_USART2[7]));
//					break;
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x02) && (usart.RxBuffer_USART2[6] == 0x11)) //位置设置
//				{
//					motor[DJID].valueSet.angle = atof((char *)(&usart.RxBuffer_USART2[7]));
//					break;
//				}
//			}
//			break;
//			case 0x03: //	elmo界面
//			{
//				if ((usart.RxBuffer_USART2[5] == 0x08) && (usart.RxBuffer_USART2[6] == 0x11)) //ID号
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x31:
//						ELMOID = 1;
//						break;
//					case 0x32:
//						ELMOID = 2;
//						break;
//					case 0x33:
//						ELMOID = 3;
//						break;
//					case 0x30:
//						ELMOID = 0;
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x09) && (usart.RxBuffer_USART2[6] == 0x10)) //开始
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					ELMOmotor[ELMOID].begin = 1;
//					break;
//					case 0x00:
//					ELMOmotor[ELMOID].begin = 0;
//					break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x0A) && (usart.RxBuffer_USART2[6] == 0x10)) //使能
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					ELMOmotor[ELMOID].enable = 1;
//					break;
//					case 0x00:
//					ELMOmotor[ELMOID].enable = 0;
//					break;
//					default:;
//					}
//				}				
//				if ((usart.RxBuffer_USART2[5] == 0x03) && (usart.RxBuffer_USART2[6] == 0x11)) //模式
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x32:
//						ELMO_motor_UM(ELMOID, 2, 0);
//						break;
//					case 0x34:
//						ELMO_motor_UM(ELMOID, 4, 0);
//						break;
//					case 0x35:
//						ELMO_motor_UM(ELMOID, 5, 0);
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x01) && (usart.RxBuffer_USART2[6] == 0x11)) //转速设置
//				{
//					ELMO_motor_JV(ELMOID, atof((char *)(&usart.RxBuffer_USART2[7])), 1);
//					ELMO_motor_BG(ELMOID, 0);
//					break;
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x02) && (usart.RxBuffer_USART2[6] == 0x11)) //位置设置
//				{
//					ELMO_motor_SP(ELMOID, atof((char *)(&usart.RxBuffer_USART2[7])), 1);
//					ELMO_motor_BG(ELMOID, 0);
//					break;
//				}
//			}
//			break;
//			case 0x02: //VESC界面
//			{
//				if ((usart.RxBuffer_USART2[5] == 0x08) && (usart.RxBuffer_USART2[6] == 0x11)) //ID号
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x31:
//						VESCID = 1;
//						break;
//					case 0x32:
//						VESCID = 2;
//						break;
//					case 0x33:
//						VESCID = 3;
//						break;
//					case 0x30:
//						VESCID = 0;
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x09) && (usart.RxBuffer_USART2[6] == 0x10)) //使能
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					VESCmotor[VESCID].enable = 1;
//					break;
//					case 0x00:
//					VESCmotor[VESCID].enable = 0;
//					break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x0A) && (usart.RxBuffer_USART2[6] == 0x10)) //开始
//				{
//					switch(usart.RxBuffer_USART2[8])
//					{
//					case 0x01:
//					VESCmotor[VESCID].begin = 1;
//					break;
//					case 0x00:
//					VESCmotor[VESCID].begin = 0;
//					break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x03) && (usart.RxBuffer_USART2[6] == 0x11)) //模式
//				{
//					switch (usart.RxBuffer_USART2[7])
//					{
//					case 0x32:
//						VESCmotor[VESCID].mode = 2;
//						break;
//					case 0x35:
//						VESCmotor[VESCID].mode = 5;
//						break;
//					default:;
//					}
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x01) && (usart.RxBuffer_USART2[6] == 0x11)) //转速设置
//				{
//					VESCmotor[VESCID].ValSet.speed = atof((char *)(&usart.RxBuffer_USART2[7]));
//					break;
//				}
//				if ((usart.RxBuffer_USART2[5] == 0x02) && (usart.RxBuffer_USART2[6] == 0x11)) //位置设置
//				{
//					VESCmotor[VESCID].ValSet.position = atof((char *)(&usart.RxBuffer_USART2[7]));
//					break;
//				}
//			}
//			case 0x05://夹拉戈里界面
//			{
//			if ((usart.RxBuffer_USART2[5] == 0x01) && (usart.RxBuffer_USART2[6] == 0x10)) 
//			{
//				//流程
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x02) && (usart.RxBuffer_USART2[6] == 0x10))
//			{
//				//位置1
//				motor[0].valueSet.angle=paw_pos[1];
//					break;

//			}
//			if ((usart.RxBuffer_USART2[5] == 0x07) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//位置1
//				motor[0].valueSet.angle= atof((char *)(&usart.RxBuffer_USART2[7]));
//					break;
//			
//			}			
//			if ((usart.RxBuffer_USART2[5] == 0x03) && (usart.RxBuffer_USART2[6] == 0x10))
//			{
//				//位置2
//				motor[0].valueSet.angle=paw_pos[2];
//					break;

//			}
//			if ((usart.RxBuffer_USART2[5] == 0x08) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//位置2
//				motor[0].valueSet.angle = atof((char *)(&usart.RxBuffer_USART2[7]));
//				break;
//			}	
//			if ((usart.RxBuffer_USART2[5] == 0x04) && (usart.RxBuffer_USART2[6] == 0x10)) 
//			{
//				//位置3
//				motor[0].valueSet.angle=paw_pos[3];
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x09) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//位置3
//				motor[0].valueSet.angle = atof((char *)(&usart.RxBuffer_USART2[7]));
//				break;
//			}	
//			if ((usart.RxBuffer_USART2[5] == 0x05) && (usart.RxBuffer_USART2[6] == 0x10)) 
//			{
//				//位置4
//				motor[0].valueSet.angle=paw_pos[4];
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x0A) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//位置4
//				motor[0].valueSet.angle = atof((char *)(&usart.RxBuffer_USART2[7]));
//				break;
//			}	
//			if ((usart.RxBuffer_USART2[5] == 0x06) && (usart.RxBuffer_USART2[6] == 0x10)) 
//			{
//				//位置5
//				motor[0].valueSet.angle=paw_pos[5];
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x0B) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//位置5
//				motor[0].valueSet.angle = atof((char *)(&usart.RxBuffer_USART2[7]));
//				break;
//			}	
//			if ((usart.RxBuffer_USART2[5] == 0x10) && (usart.RxBuffer_USART2[6] == 0x11))
//			{
//				//rpm
//				motor[0].valueSet.velocity = atof((char *)(&usart.RxBuffer_USART2[7]));
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x11) && (usart.RxBuffer_USART2[6] == 0x10))
//			{
//				motor[0].enable = 1;
//				motor[0].begin = 1;
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x0E) && (usart.RxBuffer_USART2[6] == 0x10))
//			{//零点
//				motor[0].valueSet.angle = paw_pos[0];
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x0D) && (usart.RxBuffer_USART2[6] == 0x10))
//			{
//				//电磁阀
//				break;
//			}
//			if ((usart.RxBuffer_USART2[5] == 0x11) && (usart.RxBuffer_USART2[6] == 0x10))
//			{//失能
//				motor[0].enable=0;
//				break;
//			}
//		}
//			break;
//			}
//			USART2_RX_STA = 0;
//			memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
//		}
//	}
//}

//static void USART2_Send(u8 count)
//{
//	DMA_SetCurrDataCounter(DMA1_Stream6, count);
//	DMA_Cmd(DMA1_Stream6, ENABLE);
//}

//单片机收到数据发出信息显示在串口屏上
//static char str_temp[32];
//void UsartLCDshow(void)
//{
//	u8 i = 0; //数据包的下标

//	/****主界面****/
//	usart.TxBuffer_USART2[i++] = 0xee;
//	usart.TxBuffer_USART2[i++] = 0xb1;
//	usart.TxBuffer_USART2[i++] = 0x10;
//	usart.TxBuffer_USART2[i++] = 0x00;
//	usart.TxBuffer_USART2[i++] = 0x00;
//	usart.TxBuffer_USART2[i++] = 0x00;
//	usart.TxBuffer_USART2[i++] = 0x05;
//	usart.TxBuffer_USART2[i++] = 0x10;
//	usart.TxBuffer_USART2[i++] = 0x00;
//	usart.TxBuffer_USART2[i++] = 0x01;

//	usart.TxBuffer_USART2[i++] = 0xff;
//	usart.TxBuffer_USART2[i++] = 0xfc;
//	usart.TxBuffer_USART2[i++] = 0xff;
//	usart.TxBuffer_USART2[i++] = 0xff;

//	switch (interface)
//	{
//	case 0x01: //DJ界面
//	{
//		usart.TxBuffer_USART2[i++] = 0xee;
//		usart.TxBuffer_USART2[i++] = 0xb1;
//		usart.TxBuffer_USART2[i++] = 0x12; //批量更新
//		usart.TxBuffer_USART2[i++] = 0x00; //画面ID
//		usart.TxBuffer_USART2[i++] = 0x01;
//		for (int m = 0; m < 4; m++)
//		{
//			if (motor[m].mode != 0)
//			{
//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = 0x00;
//				sprintf(str_temp, "%d", motor[m].valueReal.velocity);
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp);
//				i += strlen(str_temp);

//				usart.TxBuffer_USART2[i++] = 0x00; //控制ID
//				usart.TxBuffer_USART2[i++] = 0x02;
//				usart.TxBuffer_USART2[i++] = 0x00;				   //字节长度
//				sprintf(str_temp, "%d", motor[m].valueReal.angle); //发送格式化输出到 str 所指向的字符串
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp); //把后者所指向的字符串复制到前者
//				i += strlen(str_temp);								   //计算字符串 str_temp的长度

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x09;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = motor[m].begin;

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x0A;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = motor[m].begin;
//			}
//		}
//	}
//	break;
//	case 0x03: //elmo界面
//	{
//		usart.TxBuffer_USART2[i++] = 0xee;
//		usart.TxBuffer_USART2[i++] = 0xb1;
//		usart.TxBuffer_USART2[i++] = 0x12; //批量更新
//		usart.TxBuffer_USART2[i++] = 0x00; //画面ID
//		usart.TxBuffer_USART2[i++] = 0x03;
//		for (int m = 0; m < 4; m++)
//		{
//			if (ELMOmotor[m].mode != 0)
//			{
//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = 0x00;
//				sprintf(str_temp, "%d", ELMOmotor[m].valReal.speed1);
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp);
//				i += strlen(str_temp);

//				usart.TxBuffer_USART2[i++] = 0x00; //控制ID
//				usart.TxBuffer_USART2[i++] = 0x02;
//				usart.TxBuffer_USART2[i++] = 0x00;					 //字节长度
//				sprintf(str_temp, "%d", ELMOmotor[m].valReal.angle); //发送格式化输出到 str 所指向的字符串
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp); //把后者所指向的字符串复制到前者
//				i += strlen(str_temp);								   //计算字符串 str_temp的长度

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x09;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = ELMOmotor[m].begin;

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x0A;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = ELMOmotor[m].begin;
//			}
//		}
//	}
//	break;
//	case 0x02: //vesc界面
//	{
//		usart.TxBuffer_USART2[i++] = 0xee;
//		usart.TxBuffer_USART2[i++] = 0xb1;
//		usart.TxBuffer_USART2[i++] = 0x12; //批量更新
//		usart.TxBuffer_USART2[i++] = 0x00; //画面ID
//		usart.TxBuffer_USART2[i++] = 0x02;
//		for (int m = 0; m < 4; m++)
//		{
//			if (VESCmotor[m].mode != 0)
//			{
//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = 0x00;
//				sprintf(str_temp, "%d", VESCmotor[m].ValReal.speed);
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[m]), str_temp);
//				i += strlen(str_temp);

//				usart.TxBuffer_USART2[i++] = 0x00; //控制ID
//				usart.TxBuffer_USART2[i++] = 0x02;
//				usart.TxBuffer_USART2[i++] = 0x00;					 //字节长度
//				sprintf(str_temp, "%d", VESCmotor[m].ValReal.angle); //发送格式化输出到 str 所指向的字符串
//				usart.TxBuffer_USART2[i++] = strlen(str_temp);
//				strcpy((char *)(&usart.TxBuffer_USART2[m]), str_temp); //把后者所指向的字符串复制到前者
//				i += strlen(str_temp);								   //计算字符串 str_temp的长度

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x09;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = VESCmotor[m].begin;

//				usart.TxBuffer_USART2[i++] = 0x00;
//				usart.TxBuffer_USART2[i++] = 0x09;
//				usart.TxBuffer_USART2[i++] = 0x10;
//				usart.TxBuffer_USART2[i++] = 0x01;
//				usart.TxBuffer_USART2[i++] = VESCmotor[m].begin;
//			}
//		}
//	}
//	break;
//	default:;
//	}
//	USART2_Send(i);
//}
