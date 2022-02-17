#include "SPI2.h"

//SPI口初始化
void SPI1_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI1_InitStructure;
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //使能SPI1时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; //PB3,PB4复用功能输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                   //初始化  

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3复用为 SPI1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4复用为 SPI1
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //复位SPI1
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //停止复位SPI1

  SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
  SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;                       //设置SPI工作模式:设置为主SPI
  SPI1_InitStructure.SPI_DataSize = SPI_DataSize_16b;                  //设置SPI的数据大小:SPI发送接收16位帧结构
  SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          //串行同步时钟的空闲状态为高电平//y 数据在Dclk下降沿有效?
  SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                        //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
  SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;                           //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; //定义波特率预分频的值，SPI 速度=fAPB2/分频系数，fAPB2 时钟一般为 84Mhz：
  SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI1_InitStructure.SPI_CRCPolynomial = 7;                            //CRC值计算的多项式
  SPI_Init(SPI1, &SPI1_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

  SPI_Cmd(SPI1, ENABLE); //使能SPI1外设
}
void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI2_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能 GPIOB 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  // 使能 SPI2 时钟

    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //SPI MOSI 復用推挽GPIO_Mode_AF_PP，上拉電阻
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_AF
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP ; //GPIO_PuPd_DOWN
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //SPI MISO 浮空輸入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //GPIO_PuPd_NOPULL GPIO_PuPd_DOWN
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //SPI SCK 復用推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //GPIO_PuPd_UP
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //SPI CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //輸出信號
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //CS信號浮空
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); //PB13 复用为 SCK
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); //PB14 复用为 MISO
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); //PB15 复用为 MOSI

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE); //复位 SPI2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
		//停止复位 SPI2

    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_Direction_2Lines_FullDuplex; 		//全雙工模式
    SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;                                  //设置 SPI 工作模式:主 SPI
    SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;                              //设置 SPI 的数据大小: 8 位帧结构
    SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;                                     //时钟极性   串行同步时钟的空闲状态为高电平SPI_CPOL_High
    SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                   //时钟相位   数据捕获于第二个时钟沿SPI_CPHA_2Edge
    SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;                                      //NSS 信号由軟件管理
    SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    //SPI1_InitStructure.SPI_CRCPolynomial = 7; //CRC校验，若使用CRC，则可计算CRC的值
    SPI_Init(SPI2, &SPI2_InitStructure);

    SPI_Cmd(SPI2, ENABLE); //使能 SPI2
}

void MCP2515_Init(void)
{
    u8 message;
    MCP_RESET(); //初始化先复位芯片
    delay_ms(1);

    MCP_WRITE(RXM0SIDH, 0x00);
    MCP_WRITE(RXM0SIDL, 0x00);
    MCP_WRITE(RXF0SIDH, 0x00);
    MCP_WRITE(RXF0SIDL, 0x00); //接收屏蔽和滤波1寄存器同时置0
    MCP_WRITE(RXF0EID8, 0x00);
    MCP_WRITE(RXF0EID0, 0x00);
    MCP_WRITE(RXM0EID8, 0x00);
    MCP_WRITE(RXM0EID0, 0x00);
    /*
    Tq = (2*(BRP+1))/Fosc
    Fosc = 8MHz
    Init_Value: (f = 500k )
       T(syscseg) = 1 * Tq;
       T(proseg)  = 1 * Tq;
       T(ps1)     = 1 * Tq;
       T(ps2)     = 2 * Tq;
       SJW        = 1 * Tq;
       */
    //配置寄存器的波特率值
    MCP_WRITE(CNF1, 0x01); //SJW1 SJW0 BRP5 BRP4 BRP3 BRP2 BRP1 BRP0
    MCP_WRITE(CNF2, 0x80); //BTLMODE SAM PHSEG12 PHSEG11 PHSEG10 PRSEG2 PRSEG1 PRSEG0
    MCP_WRITE(CNF3, 0x01); //SOF WAKFIL — — — PHSEG22 PHSEG21 PHSEG20

    MCP_WRITE(RXB0CTRL, 0x64 | 0x08); //bit6-5(RXM接收工作位 11所有 10扩展 01标准 00 都)

    //设置接收0、1中断
    MCP_WRITE(CANINTF, 0x00); //清空接收0、1标志位
    MCP_WRITE(CANINTE, 0x02); //开启接收0、1中断

    MCP_WRITE(CANCTRL, REQOP_NORMAL | CLKOUT_ENABLED);

    message = (MCP_READ(CANSTAT));
    if (0x00 != (message & 0xE0))
    {
        MCP_WRITE(CANCTRL, REQOP_NORMAL | CLKOUT_ENABLED);
    }
}

void SPI2_IRQHandler(void)
{
    while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET)
    {
        SPI_ClearITPendingBit(SPI2, SPI_IT_RXNE);
        SPI_ClearFlag(SPI2, SPI_IT_RXNE);
        MCP2515_MSG Rx_message;
        if ((MCP_READ(BFPCTRL) << 6 & 0x02) == 1)
        {
            DecodeS32Data(&Rx_message.Data[0], MCP_READ(RXB0SIDH));
            DecodeS32Data(&Rx_message.Data[4], MCP_READ(RXB0SIDL));
        }
        else
        {
            DecodeS32Data(&Rx_message.Data[0], MCP_READ(RXB1SIDH));
            DecodeS32Data(&Rx_message.Data[4], MCP_READ(RXB1SIDL));
        }
    }
}

u32 MCP_READ(u32 addr)
{
    u32 READ_DATA;
    GPIOB->BSRRL = GPIO_Pin_10;
    SPI_I2S_SendData(SPI2, CAN_READ);
    SPI2_ReadWriteByte(addr);
    READ_DATA = SPI2_ReadWriteByte(0xff);
    GPIOB->BSRRH = GPIO_Pin_10;
    return READ_DATA;
}

void MCP_RESET(void)
{
    GPIOB->BSRRL = GPIO_Pin_10;
    SPI_I2S_SendData(SPI2, CAN_RESET);
    GPIOB->BSRRH = GPIO_Pin_10;
}

void MCP_WRITE(u8 addr, u8 message)
{
    GPIOB->BSRRL = GPIO_Pin_10;
    SPI_I2S_SendData(SPI2, CAN_WRITE);
    SPI2_ReadWriteByte(addr);
    SPI2_ReadWriteByte(message);
    GPIOB->BSRRH = GPIO_Pin_10;
}

u8 SPI2_ReadWriteByte(u8 TxData)
{

    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        //SPI_ClearFlag(SPI2, SPI_I2S_FLAG_TXE);
        if (retry > 200)
            return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        //SPI_ClearFlag(SPI2, SPI_IT_RXNE);
        if (retry > 200)
            return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}
