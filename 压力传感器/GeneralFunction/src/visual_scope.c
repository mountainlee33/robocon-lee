#include "visual_scope.h"
//unsigned char SendData[10];

////����ע�������ʹ�ô���1��DMA���ͣ���USART1_TX
////�ͱ���ʹ��DMA2��������7��ͨ��4
//void visualscope_init(void)
//{
//	DMA_InitTypeDef dma_initstructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//	
//	DMA_DeInit(DMA2_Stream7);														//USART1��Ӧ��DMA��ʼ��
//	dma_initstructure.DMA_Channel = DMA_Channel_4;
//	dma_initstructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);//���д���DMA���䣬�������ַΪ���ڽ��ܷ������ݴ洢�� USART1->DR �ĵ�ַ
//	dma_initstructure.DMA_Memory0BaseAddr = (u32)&SendData;	//��� DMA �������ݵ��ڴ��ַ
//	dma_initstructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	//������ڴ浽����
//	dma_initstructure.DMA_BufferSize = 10;					//һ�δ����������Ĵ�С
//	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//һֱ���̶������ַ&USART1->DR�������ݣ����Ե�ַ������
//	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//���ڴ��������洢��Ԫ�����ݷ��͵����ڣ��ڴ��ַ����Ҫ������
//	dma_initstructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//8λ�ֽڴ���
//	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//�������ڴ�����ݳ���
//	dma_initstructure.DMA_Mode = DMA_Mode_Normal;
//	dma_initstructure.DMA_Priority = DMA_Priority_High;
//	dma_initstructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
//	dma_initstructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//	dma_initstructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//���δ���
//	dma_initstructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
//	DMA_Init(DMA2_Stream7, &dma_initstructure);//��ʼ�� DMA Stream

//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
//	DMA_Cmd(DMA2_Stream7,ENABLE);
//}

////   ����ʾ����
//unsigned short CRC_CHECK(unsigned char * Buf, unsigned char CRC_CNT)
//{
//    unsigned short CRC_Temp;
//    unsigned char i, j;
//    CRC_Temp = 0xffff;

//    for (i = 0; i < CRC_CNT; i++)
//    {
//        CRC_Temp ^= Buf[i];
//        for (j = 0; j < 8; j++)
//        {
//            if (CRC_Temp & 0x01)
//                CRC_Temp = (CRC_Temp >> 1 ) ^ 0xa001;
//            else
//                CRC_Temp = CRC_Temp >> 1;
//        }
//    }
//    return(CRC_Temp);
//}



///*����ʾ�������������*/
// void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4)
//{      
//    unsigned char SendData[10];
//    unsigned short CRC16;   
//	
//	DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
//	DMA_SetCurrDataCounter(DMA2_Stream7,10);
//	
//    SendData[1] = (n_dataCH1 & 0xFF00) >> 8;	
//    SendData[0] = (n_dataCH1 & 0x00FF);
//    SendData[3] = (n_dataCH2 & 0xFF00) >> 8;
//    SendData[2] = (n_dataCH2 & 0x00FF);
//    SendData[5] = (n_dataCH3 & 0xFF00) >> 8;
//    SendData[4] = (n_dataCH3 & 0x00FF);
//    SendData[7] = (n_dataCH4 & 0xFF00) >> 8;
//    SendData[6] = (n_dataCH4 & 0x00FF);
//  
//    CRC16 = CRC_CHECK(SendData, 8);
//    SendData[8] = CRC16 % 256;
//    SendData[9] = CRC16 / 256;

//	DMA1_Channel4->CCR |= DMA_CCR1_EN;
//}
