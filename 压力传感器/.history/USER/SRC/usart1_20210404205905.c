//本次实验使用USART1

#include "usart1.h"

void USART1_MyInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	USART_DeInit(USART1);	//串口复位
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //PA9 复用为 USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 与 GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 PA9，PA10
	
	//串口参数初始化	
	USART_InitStruct.USART_BaudRate=115200;	//波特率设置
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStruct.USART_StopBits=USART_StopBits_1;	//一个停止位
	USART_InitStruct.USART_Parity=USART_Parity_No;	//奇偶校验位
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//收发模式
	USART_Init( USART1,&USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3 ; 	//抢占优先级 3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; 	//子优先级 3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 	//IRQ 通道使能
	NVIC_Init(&NVIC_InitStruct); 	//中断优先级初始化
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//当接收寄存器非空时，开启中断read register not empty(一有数据就开中断，yi)
	USART_Cmd(USART1,ENABLE);
	
}

/*当接收到从电脑发过来的数据，把接收到的数据保存在 USART1_RX_BUF 中
同时在接收状态寄存器（USART1_RX_STA）中计数接收到的有效数据个数*/

u8 USART1_RX_STA=0;	//类似于接收状态寄存器，1~6位计数，第7位置1表示接收到“#”，第8位置1表示
u8 USART1_RX_BUF[32];//接收数据
u16 len_1;

/*规定接收数据以#起始，回车结束*/

void USART1_IRQHandler()	//串口 1 的中断响应函数
{
	u8 temp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//接收寄存器非空read register not empty
	{
		USART_ClearFlag(USART1,USART_IT_RXNE);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		temp=USART_ReceiveData(USART1);//读取接收到的数据
		
 /*2*/	if((USART1_RX_STA & 0x40) != 0) //（第7位为1）即接收到“#”后
        {
 /*5*/      if((USART1_RX_STA & 0x80) == 0)	//USART1_RX_STA显示接收未完成
            {
 /*4*/          if(temp == 0x0d)//回车ASCII码，接收到回车
                    USART1_RX_STA |=0x80; //完成数据接收，把USART1_RX_STA高bit[15]置1
 /*3*/          else 
                {
                    USART1_RX_BUF[USART1_RX_STA & 0X3F] = temp;
                    USART1_RX_STA++;
					len_1++;
                    if((USART1_RX_STA & 0X3F) > 31)	//注意USART1_RX_STA高两位为标记位，真正有效计数为低6位
                        USART1_RX_STA = 0; 
                }
            }
        }
 /*1*/  else if(temp == '#') 
        {
		int i;
		USART1_RX_STA |= 0x40;	//则把USART1_RX_STA第7位置1
		for(i = 0; i < 32; i++)	//清空数组		
		USART1_RX_BUF[i] = 0x00;
         }
 				
 /*6*/	 if((USART1_RX_STA&0x80)!=0)		//数据终于全部送到数组啦，现在是转送串口		
		{		
			int i;
			for(i=0;i<len_1;i++)
			{
				USART_SendData(USART1,USART1_RX_BUF[i]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				
			}
			USART1_RX_STA=0;//清空清空
			len_1 = 0;
		} 
		}
	
}      


