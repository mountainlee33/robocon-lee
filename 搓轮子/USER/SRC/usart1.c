#include "usart1.h"
u8 usart1_tx[32];              //��������

void Usart1_Init(void)
{
USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	DMA_DeInit(DMA2_Stream7);
	DMA_InitStructure.DMA_Channel =DMA_Channel_4;
	DMA_InitStructure.DMA_BufferSize =0;
	DMA_InitStructure.DMA_DIR =DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(usart1_tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc =DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority =DMA_Priority_VeryHigh;//����Ϊf4����
	DMA_InitStructure.DMA_FIFOMode =DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold =DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst =DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst =DMA_MemoryBurst_Single;
	DMA_Init(DMA2_Stream7,&DMA_InitStructure);
	
	DMA_ClearFlag (DMA2_Stream7,DMA_IT_TCIF7);					//����жϱ�־
	DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);//�ر���DMA��������жϣ����������޷����䡣10/14/2018
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void USART1_sendData(u8 *a,u8 count)
{
	if(flag.Usart1DMASendFinish==0)//����һ�δ���δ��ɣ����������δ���
	{
	  memcpy(&usart1_tx[0],a,count);
		DMA_SetCurrDataCounter(DMA2_Stream7,count);
		DMA_Cmd(DMA2_Stream7, ENABLE);
	  flag.Usart1DMASendFinish=1;
	}
}

void DMA2_Stream7_IRQHandler(void)               //���ݴ�����ɣ������жϣ�����Ƿ���û�д�������ݣ���������
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)==SET)
	{
		DMA_ClearFlag (DMA2_Stream7,DMA_IT_TCIF7);					//����жϱ�־
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
		flag.Usart1DMASendFinish=0;
	}
}

u8 USART1_RX_STA = 0; //�����ڽ���״̬�Ĵ�����1~6λ��������7λ��1��ʾ���յ���#������8λ��1��ʾ
u8 USART1_RX_BUF[32]; //��������

/*�涨����������#��ʼ���س�����*/

void USART1_IRQHandler(void)
{
	u8 temp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���ռĴ����ǿ�
	{
		USART_ClearFlag(USART1, USART_IT_RXNE); 				//USART_FLAG_RXNE        //����жϱ�־
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		temp = USART_ReceiveData(USART1);
		if((USART1_RX_STA & 0x40) != 0) //�����Ѿ���ʼ
		{
			if((USART1_RX_STA & 0x80) == 0) //����δ���
			{
				if(temp == 0x0d)
					USART1_RX_STA |=0x80;  //���������
				else //��û�յ�*
				{
					USART1_RX_BUF[USART1_RX_STA & 0X3F] = temp;
					USART1_RX_STA++;
					if((USART1_RX_STA & 0X3F) > 32)
						USART1_RX_STA = 0; //�������ݴ������¿�ʼ����
				}
			}
		}
		else if(temp == '#')    //���յ���ͷ
			USART1_RX_STA |= 0x40;

		if((USART1_RX_STA & 0x80) != 0)     //�����Ѿ���ɣ�������������
		{	
			USART1_RX_STA = 0;
		}
	}
}

unsigned short CRC_CHECK(unsigned char * Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i, j;
    CRC_Temp = 0xffff;

    for (i = 0; i < CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];
        for (j = 0; j < 8; j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >> 1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}



/*����ʾ�������������*/
 void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4)
{      
    unsigned char SendData[10];
    unsigned short CRC16;   
    SendData[1] = (n_dataCH1 & 0xFF00) >> 8;
    SendData[0] = (n_dataCH1 & 0x00FF);
    SendData[3] = (n_dataCH2 & 0xFF00) >> 8;
    SendData[2] = (n_dataCH2 & 0x00FF);
    SendData[5] = (n_dataCH3 & 0xFF00) >> 8;
    SendData[4] = (n_dataCH3 & 0x00FF);
    SendData[7] = (n_dataCH4 & 0xFF00) >> 8;
    SendData[6] = (n_dataCH4 & 0x00FF);
  
    CRC16 = CRC_CHECK(SendData, 8);
    SendData[8] = CRC16 % 256;
    SendData[9] = CRC16 / 256;

	USART1_sendData(SendData,10);
}

