## STM芯片学习

### 寄存器学习

寄存器變成：利用指針取地址(宏定義)中的數據，地址需要强制類型轉換                                          
SP(stack pointer堆棧指針)寄存器R13，0=主堆棧指針(MSP),1=進程堆棧指針(PSP)                                              keil编译信息
code:程序所占用的flash的大小                                                                                                                   
RO-data：即 Read Only-data，表示程序定义的常量，如 const 类型（FLASH）。
RW-data：即 Read Write-data，表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data，表示未被初始化的全局变量(SRAM)

120的终端电阻，做阻抗匹配，以减少回波反射

時鐘樹：
时钟是单片机运行的基础，时钟信号推动单片机内各个部分执行相应的指令.主要的目的就是给相对独立的外设模块提供时钟，也是为了降低整个芯片的耗能。
HSI是高速内部时钟，RC振荡器，频率为8MHz，精度不高。
HSE是高速外部时钟，可接石英/陶瓷谐振器，或者接外部时钟源，频率范围为4MHz~16MHz。
LSI是低速内部时钟，RC振荡器，频率为40kHz，提供低功耗时钟.
LSE是低速外部时钟，接频率为32.768kHz的石英晶体。
系统时钟通过AHB分频器给外设提供时钟:系统时钟--->AHB分频器--->各个外设分频倍频器 --->外设时钟的设置
PLL:Phase Locked Loop 鎖相環  用于振荡器中的反馈技术，通常需要外部的输入信号与内部的振荡信号同步
PLL = 外部晶振 *N / (M*P)

bxCAN工作模式：初始化、正常和睡眠
初始化CAN_MCR 寄存器的 INRQ置 1
睡眠 CAN_MCR(can master control register) 寄存器的 SLEEP 置 1
进入正常模式的请求可通过将 CAN_MCR 寄存器的 INRQ 位清零
CAN_TIxR 寄存器的相应 TXRQ 位置 1，选择一个空发送邮箱，并设置标识符、数据长度代码 (DLC) 和数据
初始化模式，MCR寄存器的INRQ位置1，并通过将MCR寄存器的INAK位置1来确认请求
初始化can，必须设置位寄存器(CAN_BTR(can bit timing register))和CAN选项(CAN_MCR).
CAN_TSR(can transmit register) 寄存器的 RQCP 和 TXOK 位置 1表示发送成功
失败原因将由 CAN_TSR 寄存器的 ALST 位（仲裁丢失）和/或 TERR 位（检 
测到发送错误）指示
筛选器组通过将 CAN_FAR寄存器的FACT位清零而停用，通过 CAN_FS1R 寄存器的相应FSCx位进行配置
CAN发送邮箱标识符寄存器(CAN_TIxR:CAN TX mailbox identifier register)： 存储待发送报文的 ID、扩展 ID、IDE 位及 RTR 位 
CAN邮箱数据低位寄存器(CAN_TDLxR)、CAN邮箱数据高位寄存器(CAN_TDHxR)
时间片：分时操作系统分配给每个正在运行的进程微观上的一段CPU时间
位时序：同步段( SYNC SEG) ：位变化在此时间段发生，只有一个时间片的固定长度； 相位缓冲段 1：定义采样点的位置(持续长度可以在1到16个时间片), 主要用来补偿边沿阶段的误差 ； 相位缓冲段 2：定义发送点的位置(1到8个时间片); PBS1 和 PBS2 中增加或减少的这段时间长度被定义为“重新同步补偿宽度 SJW 
BRP[9:0]、TS1[3:0]和TS2[2:0]在CAN_BTR寄存器中定义
SOF=帧起始、ID=标识符、RTR=远程传输请求、IDE=标识符扩展位、r0=保留位、DLC=数据长度代码、EOF=帧结束、ACK=确认位
CAN中断使能寄存器(CAN_IER(can interrupt enable register))，四个中断：发送中断、FIFO0中断、FIFO1中断、错误和状态改变中断
发送优先级：可以通过设置 CAN_MCR 寄存器中的 TXFP 位，将发送邮箱配置为发送 FIFO。在此模式下， 优先级顺序按照发送请求顺序来确定

![1642220217186](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642220217186.png)

FIFO(先进先出存储器缓冲区)管理：

DMA(direct memory access直接存储器)用于高速数据传输，2个DMA控制器16个数据流，各8个通道.
具体通道和数据流看参考手册
DMA中断状态寄存器，DMA_LISR和DMA_HISR，标志位：传输完成中断，半传输中断，传输错误中断，FIFO错误中断
DMA中断标志清除寄存器DMA_LIFCR和DMA_HIFCR，上面两个寄存器分别控制4个数据流
DMA数据流x配置寄存器(DMA_SxCR)，数据宽度、存储器宽度、优先级、增量模式、传输方向、中断允许、使能
DMA数据流x数据项数寄存器(DMA_SxNDTR)，控制数据流x的每次传输的数据量
DMA数据流x的外设地址寄存器(DMA_SxPAR)，存储外设的地址
DMA数据流x的存储器地址寄存器，有两个DMA_SxM0AR和DMA_SxM1AR

ADC是12位逐次逼近型的模拟数字转换器，ADC的转换分为2个通道组：规则通道组(正常运行的程序)和注入通道组(中断)
ADC控制寄存器(ADC_CR1和ADC_CR2)，ADC通用控制寄存器(ADC_CCR),ADC规则序列寄存器(ADC_SQR1~3)
ADC采样时间寄存器(ADC_SMPR1和ADC_SMPR2)，Tcovn(总转换时间)=采样时间(每个通道的SMP位的设置)+12个周期，
ADC规则序列寄存器(ADC_SQR1~3),L[3：0]用于存储规则序列的长度
ADC规则数据寄存器(ADC_DR),AD转化结果存在这个寄存器里面，且保存在ADC_JDRx里
ADC状态寄存器(ADC_SR)

ITM：儀器跟蹤宏單元數據觀察點和分析單元

高级定时器：①计数器16bit，上、下、两边计数，TIM1和TIM8，还有一个重复计数器RCR(独有)；②有4个GPIO，1~3有互补输出GPIO；③时钟来自PCLK2，为72M，可实现1~65536分频
时钟源：内部时钟(CK_INT),外部时钟1：外部输入引脚，外部时钟2：外部触发输入ETR，内部触发输入(ITRX):使用一个定时器作为另一个定时器的预分频器
时基单元：计数器寄存器(TIMx_CNT)、预分频寄存器(TIMx_PSC)、自动重载寄存器(TIMx_ARR)、重复计数器寄存器(TIMx_RCR)

系統定時器：SysTick是一個24位倒計時定時器，將此用作定時操作系統(RTOS)滴答計時器

定时器：

tim1、8、10、11使用系统时钟，其他使用一半时钟
基本定时器时钟只能来自内部时钟，高级控制定时器和通用 定时器还可以选择外部时钟源或者直接来自其他定时器等待模式 
定时器基本初始化结构体TIM_TimeBaseInitTypeDef
TIM_Prescaler定时器预分频设置，TIM_CounterMode选择了计数器模式，TIM_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
定时器比较输出初始化结构体 TIM_OCInitTypeDef
TIM_OCMode比较输出模式选择，共8种，设置TIM_Pulse的值就可以改变输出波形的占空比
定时器输入捕获初始化结构体  TIM_ICInitTypeDef
TIM_Channel输入通道选择，共4个通道；TIM_ICPrescaler输入捕获通道预分频，共（1、2、4、8）种
TIM_ICFilter输入捕获滤波器设置	
断路和死区初始化结构体TIM_BDTRInitTypeDef
TIM_CCxCmd()单独关闭与开启一路通道

影子寄存器、预装载寄存器、TIM_OC1PreloadConfig和TIM_ARRPreloadConfig的作用
有影子寄存器：PSC预分频器、自动重装载寄存器、REP寄存器和4个通道的捕获/比较寄存器。
TIM_ARRPreloadConfig(TIM1, ENABLE)函数的作用：
	这两种方式主要取决于寄存器TIMx->CR1中的“APRE”位：
	APRE=0，当ARR值被修改时，同时马上更新影子寄存器的值；
	APRE=1，当ARR值被修改时，必须在下一次事件UEV发生后才能更新影子寄存器的值；

TIM_SetCompare2(TIM_TypeDef TIMx,uint16_t Compare2)
2代表的是TIMx的通道2，TIMx中的x可以取1、2、3、4、5、6、8、9、12、15。
Compare2给了捕获/比较寄存器一个捕获值;
TIMx->CCR2，捕获/比较寄存器一共有四个(TIMx->CCR1-4)，用到TIMx的通道二，与TIM_SetCompare2对应；
如何输出PWM波TIM3_PWM_Init(u16 arr,u16 psc)中arr:自动重载值

GPIO:4个32位配置寄存器(GPIOx_MODER、GPIOx_OTYPER、GPIOx_OSPEEDR和GPIOx_PUPDR)、2个32位数据寄存器(GPIOx_IDR和GPIOx_ODR)、1个32位置位/复位寄存器(GPIOx_BSRR)、1个32位锁定寄存器(GPIOx_LCKR)(可冻结I/O配置)和2个32位复用功能选择寄存器(GPIOx_AFRH和GPIOX_AFRL)
I/O 端口控制寄存器：GPIOx_MODER用于选择I/O方向(输入、输出、AF、模拟)，GPIOx_OTYPER选择输出类型(输出类型)，GPIOx_OSPEEDR选择速度
I/O 端口数据寄存器：GPIOx_ODR 用于存储待输出数据，可对其进行读/写访问；GPIOx_IDR是一个只读寄存器
GPIOx_BSRR允许对GPIOx_ODR对单独的数据进行置位和复位操作，可以随时访问GPIOx_ODR位

NVIC:嵌套向量中斷控制器，支持低延遲中斷架構

![1642216256876](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642216256876.png)



 USART 数据寄存器 (USART_DR) 只有低 9 位有效，并且第 9 位数据是否有效要取决于 USART 控制寄存器 1(USART_CR1) 的 M 位设置，当 M 位为 0 时表示 8 位数据字长 
 USART_DR 包含 专门用于发送的可写 TDR和专门用于接收的可读 RDR 。 支持 DMA 传输 

## ![1642216471488](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642216471488.png)USART終端請求

## HAL库学习

cortex m3、m4、m7 差异体现：指令集、浮点运算、数字信号处理；是否有缓存cache、TCM；性能的差异；功耗

MCU(固件包)：例程、中间件、HAL库

STM32Cube——FW_F4——：Documents、Drivers、Middleware、Projects、Utilites

Drivers:CMSIS（Cortex软件接口标准：内核寄存器定义、启动文件等）、BSP、STM32F4xx_HAL_dRIVER

![1642206531055](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642206531055.png)

SYSTEM CORE:用于配置GPIO、时钟源、中断系统以及系统相关的外设

Analog：用于配置模、数和数、模转换外设

Timers：用于配置定时器和实时时钟外设

Connectivity：用于配置I2C、SPI和UART等连接外设

Multimedia：用于配置I2S等音频数据传输外设

computing：crc校验外设

外设资源选择
外设配置-可以设置你所选择外设的各种功能
和我们配置库函数的时候基本一样，但是  STM32CubeMx将这些转换为了图形和选项
系统预览就是查看配置的各种外设和GPIO的状态 

时钟频率一定要配对

disable:内部时钟、BYPASS:旁路时钟、Crystal：晶振

![1642209793348](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642209793348.png)

ST意法半导体 M微电子 32总线宽度 MCU單片機(集成電路芯片，采用超大規模集成電路把CPU、隨機存儲器RAM、只讀存儲器ROM、多種I/O口、定時器/計數器等功能集成到一個微型計算機系統)

引脚分配：電源IO(VBAT、VDD VSS、VDDA VSSA、VREF+ VREF-)；晶振IO：主晶振IO，RTC晶振IO；BOOT IO：BOOT0,BOOT1,用於設置系統的啓動方式；復位IO：NRST，用於設置系統的啓動方式；通用輸入輸出IO

![1642210573467](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642210573467.png)

初始化的燒錄jlink口一定要復用，cannot read jlink version number解決方法：按住板子reset鍵，直至識別到。將程序中有關jlink的信息全部刪除

GPIO output level初始电平、GPIO mode引脚模式、GPIO Pull-up、Pull-down上/下拉电阻、Maximum output speed引脚速度

gpio通過2個mos管組成推挽(驅動能力強)、開漏(通過上拉電阻實現，實現雙向通信)或關閉模式

![1642212168302](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642212168302.png)



CAN_HandleTypeDef{
 CAN_TypeDef *Instance; //位寄存器基地址
 CAN_InitTypeDef Init; 
 CanTxMsgTypeDef* pTxMsg; //发送消息结构体指针
 CanRxMsgTypeDef* pRxMsg; //接收消息结构体指针
 __IO HAL_CAN_StateTypeDef State; 
 HAL_LockTypeDef Lock; 
 __IO uint32_t ErrorCode;}
CAN_InitTypeDef{
uint32_t Prescaler;
 uint32_t Mode; 
 uint32_t SJW; 
 uint32_t BS1; 
 uint32_t BS2; 
 uint32_t TTCM;
 uint32_t ABOM; 
 uint32_t AWUM;
 uint32_t NART; 
 uint32_t RFLM;
 uint32_t TXFP; }
 前5个参数设置寄存器CAN_BTR，用来设置模式以及波特率相关的参数

#### ERROR：

error：invalid rom table 设置错误的时钟频率
将下载速度更改为100kHz,用CMSIS蓝牙下载，flash Download 中选用Erase full chips，
program，verify，rest and run全部取消勾选
error：新增加函数出现Undefined symbol HAL_XXX_Init， 打開库文件：stm32l0xx_hal_conf.h ，解除注釋對應的模塊

## SPI转can

SPI標準的讀寫指令以及專門的SPI命令來讀/寫所有的寄存器
![1642354039042](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642354039042.png)
MCP2515 有三个发送缓冲器和两个接收缓冲器，两个 验收屏蔽寄存器（分别对应不同的接收缓冲器）以及六 个验收滤波寄存器 
核心是有限状态机 Finite State Machine
SOF之后是仲裁字段，由12个位组成，分别为11个标识位和一个远程发送请求(RTR):用來區分數據幀還是遠程幀
控制字段，由 6 个位组成。
控制字段的第一位为标识扩展（IDE）位，该位应是显性状态来指定标准帧。 标识扩展位的下一位为零保留位RB0，CAN 协议将其定义为显性位。控制字段的其余 4 位为数据长度码Data Length Code(DLC).
  控制字段之后为数据字段，包含要发送的任何数据字节 

##### 發送緩衝器

 TXBnSIDH、TXBnSIDL( 标准标识符的高低位);TXBnDLC( 数据长度码 )；TXBnDm 寄存器(數據)
 在报文发送之前， MCU 应对 CANINTE.TXInE 位进行初始化，以便在报文发送时使能或禁止中断的产生 。在写入发送缓冲器之前，必须将TXBnCTRL.TXREQ 位清零。

##### 接受緩衝器

 两个全接收缓冲器。每个接收缓冲器配 备有多个验收滤波器； 单独的报文集成缓冲器MAB总能够接收来自总线的下一条报文，並傳送至RXB0和RXB1緩衝器
 RXB1 是优先级较低的缓冲器，配置有一个屏蔽滤波寄 存器和四个验收滤波寄存器 
 RXB0 是具有较高优先级的缓冲器，配置有一个屏蔽滤 波寄存器和两个验收滤波寄存器 
![1642357456590](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642357456590.png)

 相位缓冲段 1(PS1)和相位缓冲段 2(PS2) 

#### 中斷

 建议在对 CANINTF 寄存器中的标志位进行复位操作。
 接收中断使能 CANINTE.RXnIE = 1 

 MCP2515有五种工作模式： 1. 配置模式 2. 正常模式 3. 休眠模式 4. 仅监听模式 5. 环回模式

只有在配置模式下，才能对以下寄存器进行修改： CNF1、CNF2 和 CNF3  TXRTSCTRL  验收过滤寄存器 验收屏蔽寄存器 

#### SPI接口

 外部数据和命令通过SI引脚传送到器件中，且数据在 SCK 时钟信号的上升沿传送进去.  MCP2515 在 SCK 的下降沿通过 SO 引脚传送出去。在进行任何操作时， CS引脚都必须保持为低电平。 
CS引脚必须先拉升为高电平然后再降为低电平以调用另外一个命令.
复位指令为单字节指令。应先将 CS 引脚置为低电平来选中器件，随后发送命令字节，发送完毕之后再将 CS 引脚拉升为高电平 
RX状态指令用于快速确定与报文和报文类型（标准帧、扩展帧或远程帧）相匹配的滤波器 
SCK: Serial Clock   Master 设备往 Slave 设备传输时钟信号, 控制数据交换的时机以及速率 
CS :  用于 Master 设备片选 Slave 设备, 使被选中的 Slave 设备能够被 Master 设备所访问 
MOSI： 在 Master上面也被称为Tx-Channel, 作为数据的出口, 主要用于 SPI 设备发送数据 
MISO： 在 Master上面也被称为Rx-Channel, 作为数据的入口, 主要用于SPI 设备接收数据 

#### SPI指令集

 复位 1100 0000 将内部寄存器复位为缺省状态，并将器件设定为配置模式。 
 读     0000 0011 从指定地址起始的寄存器读取数据。
 写     0000 0010 将数据写入指定地址起始的寄存器。
  可以同时发出和接收串行数据；可以当作主机或从机工作；提供频率可 编程时钟；发送结束中断标志；写冲突保护；总线竞争保护 

## SPI

指令应先读取之前寄存器的信息，再修改寄存器某一位处的信息
使用DMA進行SPI通信，不斷提供要發送的數據，且及時接取緩衝區的數據
中斷：RXNE( 接收缓冲区非空 )、 TXE( 发送缓冲区为空)、
SPI特性：基於三條綫的全雙工同步傳輸，基於雙綫的單工同步傳輸、 具有 DMA 功能的 1 字节发送和接收缓冲器：发送和接收请求 
I2S特性：全雙工通信、半雙工通信(僅作爲發送器或接收器)

## SPI配置成主器件

1.设置 BR[2:0] 位以定义串行时钟波特率（参见 SPI_CR1 寄存器）
2.选择 CPOL 和 CPHA 位，以定义数据传输和串行时钟之间的关系 
3.设置 DFF 位，以定义 8 或 16 位数据帧格式
4.配置 SPI_CR1 寄存器中的 LSBFIRST 位以定义帧格式。如果选择 TI 模式，则不需要此步骤。
5.如果 NSS 引脚配置成输入，在 NSS 硬件模式下，NSS 引脚在整个字节发送序列期间都连接到高电平信号；在 NSS 软件模式下，将 SPI_CR1 寄存器中的 SSM 和 SSI 位置  1。如果 NSS 引脚配置成输出，只应将 SSOE 位置 1。如果选择 TI 模式，则不需要此步骤。 
将 SPI_CR2 中的 FRF 位置 1，以选择 TI 协议进行串行通信。MSTR 和 SPE 位必须置 1（仅当 NSS 引脚与高电平信号连接时，这两个位才保持置 1） 

##### SPI中的极性CPOL和相位CPHA

CKPOL (Clock Polarity) = CPOL = POL = Polarity = （时钟）极性 表示当SCLK空闲idle的时候，其电平的值是低电平0还是高电平1 
CKPHA (Clock Phase)  = CPHA = PHA = Phase = （时钟）相位  数据采样，数据有效的时刻 
SCK=SCLK=SPI的时钟
Edge=边沿，即时钟电平变化的时刻，即上升沿(rising edge)或者下降沿(falling edge) 

## I2C

  Inter IC Sound总线:采用了沿独立的导线传输时钟与数据信号的设计，通过将数据和时钟信号分离，避免了因时差诱发的失真

 I2S总线主要有3个信号线：
串行时钟SCK  SCK的频率 = 声道数 * 采样频率 * 采样位数 
字段选择信号WS，也叫LRCLK，用于切换左右声道的数据。WS的频率 = 采样频率 
串行数据SD，就是用二进制补码表示的音频数据。I2S串行数据在传输的时候，由高位(MSB)到低位(LSB)依次进行传输 

![1642490859035](C:\Users\mountainlee_33\AppData\Roaming\Typora\typora-user-images\1642490859035.png)

 采样、量化、编码为模拟量转换成数字量的三个过程

