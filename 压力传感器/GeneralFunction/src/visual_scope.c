#include "visual_scope.h"
//unsigned char SendData[10];

////这里注意查表，如果使用串口1的DMA发送，即USART1_TX
////就必须使用DMA2的数据流7，通道4
//void visualscope_init(void)
//{
//	DMA_InitTypeDef dma_initstructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//	
//	DMA_DeInit(DMA2_Stream7);														//USART1相应的DMA初始化
//	dma_initstructure.DMA_Channel = DMA_Channel_4;
//	dma_initstructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);//进行串口DMA传输，外设基地址为串口接受发送数据存储器 USART1->DR 的地址
//	dma_initstructure.DMA_Memory0BaseAddr = (u32)&SendData;	//存放 DMA 传输数据的内存地址
//	dma_initstructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	//方向从内存到外设
//	dma_initstructure.DMA_BufferSize = 10;					//一次传输数据量的大小
//	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//一直往固定外设地址&USART1->DR发送数据，所以地址不递增
//	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//将内存中连续存储单元的数据发送到串口，内存地址是需要递增的
//	dma_initstructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//8位字节传输
//	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//来设置内存的数据长度
//	dma_initstructure.DMA_Mode = DMA_Mode_Normal;
//	dma_initstructure.DMA_Priority = DMA_Priority_High;
//	dma_initstructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
//	dma_initstructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//	dma_initstructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//单次传输
//	dma_initstructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
//	DMA_Init(DMA2_Stream7, &dma_initstructure);//初始化 DMA Stream

//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
//	DMA_Cmd(DMA2_Stream7,ENABLE);
//}

////   两个示波器
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



///*虚拟示波器的输出函数*/
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
