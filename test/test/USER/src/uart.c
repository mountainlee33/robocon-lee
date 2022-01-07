#include "uart.h"
//������1��6ʱ��ʹ����APB2ENR�Ĵ�������������ʱ��ʹ�ܶ���APB1ENR�Ĵ���
//����������(USART_BRR)������λ���ȣ���żУ��λ
//������= fpclkx /(16*USARTDIV),APB2����ʱ��Ƶ��Ϊ84M
//���ڷ��ͽ���ͨ��USART_DR(˫�Ĵ���)
//����״̬(״̬�Ĵ��� USART_SR):RXNE=1,���ݽ��յ��ˣ�TC=1,�����Ѿ��������
//f4��һ������������������λ��OVER8=1���ݴ��ٶȿ죻OVER8=0,���ȸߣ��ݴ��Ժ�
void UART2_Init(u32 pclk2, u32 bound) //bound:�����ʣ�pclk2:APB2����ʱ��Ƶ��(Mhz)
{
    float uart_baud;
    u16 entirety;
    u16 dot;
    uart_baud = (float)(pclk2 * 1000000) / (bound * 16);
    entirety = uart_baud;              //�õ���������
    dot = (uart_baud - entirety) * 16; //�õ�С������
    entirety <<= 4;
    entirety += dot;
    RCC->AHB1ENR = 1 << 0; //ʹ��portʱ��
    RCC->APB2ENR = 1 << 4; //���Ǵ���1ʱ�ӣ�����2ʱ����ȷ��
    GPIO_Set(GPIOA, PIN2 | PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP,
             GPIO_SPEED_100M, GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOA, 2, 2); //PA2,AF2
    GPIO_AF_Set(GPIOA, 3, 2); //PA3,AF2
    //����������
    USART2->BRR = entirety;    //����������
    USART2->CR1 &= ~(1 << 15); //����OVER8=0
    USART2->CR1 = 1 << 3;      //���ڷ���ʹ��
#if EN_USART2_RX               //���ʹ���˽���
    USART2->CR1 = 1 << 2;      //���ڽ���ʹ��
    USART2->CR1 = 1 << 5;      //���ջ������ǿ��ж�ʹ��
    MY_NVIC_Init(4��1��USART2_IRQn, 2);
#endif
    USART2->CR1 = 1 << 13; //����ʹ��
}
//����״̬�Ĵ��� USART_RX_STA,USART_RX_BUF �Ĵ�С�� USART_REC_LEN ����
const int USART_REC_LEN = 256;  //���շ��ͻ������Ϊ����
u8 USART_RX_BUF[USART_REC_LEN]; //���ջ��壬���USART_REC_LEN���ֽ�
u16 USART2_RX_STA = 0;          //����״̬���
void UART2_IRQHandler(void)
{
    u8 res;
    if (USART2->SR & (1 << 5)) //���յ�������
    {
        res = USART2->DR;
        if ((USART2_RX_STA & 0x8000) == 0) //����δ���
        {
            if (USART2_RX_STA & 0x4000) //���յ���0x0d
            {
                if (res != 0x0a)
                    USART2_RX_STA = 0; //���մ������¿�ʼ
                else
                    USART2_RX_STA |= 0x8000; //���������
            }
            else
            {
                USART_RX_BUF[USART2_RX_STA & 0X3FFF] = res;
                USART2_RX_STA++;
                if (USART2_RX_STA > (USART_REC_LEN - 1))
                    USART2_RX_STA = 0; //�������ݴ������¿�ʼ����
            }
        }
    }
}
