#include "uart.h"
//除串口1，6时钟使能在APB2ENR寄存器，其他串口时钟使能都在APB1ENR寄存器
//波特率设置(USART_BRR)，数据位长度，奇偶校验位
//波特率= fpclkx /(16*USARTDIV),APB2总线时钟频率为84M
//串口发送接收通过USART_DR(双寄存器)
//串口状态(状态寄存器 USART_SR):RXNE=1,数据接收到了，TC=1,数据已经发送完成
//f4多一个接收器过采样设置位：OVER8=1，容错差，速度快；OVER8=0,精度高，容错性好
void UART2_Init(u32 pclk2, u32 bound) //bound:波特率，pclk2:APB2总线时钟频率(Mhz)
{
    float uart_baud;
    u16 entirety;
    u16 dot;
    uart_baud = (float)(pclk2 * 1000000) / (bound * 16);
    entirety = uart_baud;              //得到整数部分
    dot = (uart_baud - entirety) * 16; //得到小数部分
    entirety <<= 4;
    entirety += dot;
    RCC->AHB1ENR = 1 << 0; //使能port时钟
    RCC->APB2ENR = 1 << 4; //这是串口1时钟，串口2时钟再确认
    GPIO_Set(GPIOA, PIN2 | PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP,
             GPIO_SPEED_100M, GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOA, 2, 2); //PA2,AF2
    GPIO_AF_Set(GPIOA, 3, 2); //PA3,AF2
    //波特率设置
    USART2->BRR = entirety;    //波特率设置
    USART2->CR1 &= ~(1 << 15); //设置OVER8=0
    USART2->CR1 = 1 << 3;      //串口发送使能
#if EN_USART2_RX               //如果使能了接受
    USART2->CR1 = 1 << 2;      //串口接收使能
    USART2->CR1 = 1 << 5;      //接收缓冲区非空中断使能
    MY_NVIC_Init(4，1，USART2_IRQn, 2);
#endif
    USART2->CR1 = 1 << 13; //串口使能
}
//接收状态寄存器 USART_RX_STA,USART_RX_BUF 的大小由 USART_REC_LEN 定义
const int USART_REC_LEN = 256;  //接收发送缓存必须为常数
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲，最大USART_REC_LEN个字节
u16 USART2_RX_STA = 0;          //接收状态标记
void UART2_IRQHandler(void)
{
    u8 res;
    if (USART2->SR & (1 << 5)) //接收到了数据
    {
        res = USART2->DR;
        if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART2_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)
                    USART2_RX_STA = 0; //接收错误，重新开始
                else
                    USART2_RX_STA |= 0x8000; //接收完成了
            }
            else
            {
                USART_RX_BUF[USART2_RX_STA & 0X3FFF] = res;
                USART2_RX_STA++;
                if (USART2_RX_STA > (USART_REC_LEN - 1))
                    USART2_RX_STA = 0; //接收数据错误，重新开始接收
            }
        }
    }
}
