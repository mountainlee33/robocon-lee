//����ʵ��ʹ��USART1

#include "usart1.h"

void USART1_MyInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	USART_DeInit(USART1);	//���ڸ�λ
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //PA9 ����Ϊ USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 �� GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ�� PA9��PA10
	
	//���ڲ�����ʼ��	
	USART_InitStruct.USART_BaudRate=115200;	//����������
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStruct.USART_StopBits=USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStruct.USART_Parity=USART_Parity_No;	//��żУ��λ
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//�շ�ģʽ
	USART_Init( USART1,&USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3 ; 	//��ռ���ȼ� 3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; 	//�����ȼ� 3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 	//IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct); 	//�ж����ȼ���ʼ��
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ռĴ����ǿ�ʱ�������ж�read register not empty(һ�����ݾͿ��жϣ�yi)
	USART_Cmd(USART1,ENABLE);
	
}

/*�����յ��ӵ��Է����������ݣ��ѽ��յ������ݱ����� USART1_RX_BUF ��
ͬʱ�ڽ���״̬�Ĵ�����USART1_RX_STA���м������յ�����Ч���ݸ���*/

u8 USART1_RX_STA=0;	//�����ڽ���״̬�Ĵ�����1~6λ��������7λ��1��ʾ���յ���#������8λ��1��ʾ
u8 USART1_RX_BUF[32];//��������
u16 len_1;

/*�涨����������#��ʼ���س�����*/

void USART1_IRQHandler()	//���� 1 ���ж���Ӧ����
{
	u8 temp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//���ռĴ����ǿ�read register not empty
	{
		USART_ClearFlag(USART1,USART_IT_RXNE);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		temp=USART_ReceiveData(USART1);//��ȡ���յ�������
		
 /*2*/	if((USART1_RX_STA & 0x40) != 0) //����7λΪ1�������յ���#����
        {
 /*5*/      if((USART1_RX_STA & 0x80) == 0)	//USART1_RX_STA��ʾ����δ���
            {
 /*4*/          if(temp == 0x0d)//�س�ASCII�룬���յ��س�
                    USART1_RX_STA |=0x80; //������ݽ��գ���USART1_RX_STA��bit[15]��1
 /*3*/          else 
                {
                    USART1_RX_BUF[USART1_RX_STA & 0X3F] = temp;
                    USART1_RX_STA++;
					len_1++;
                    if((USART1_RX_STA & 0X3F) > 31)	//ע��USART1_RX_STA����λΪ���λ��������Ч����Ϊ��6λ
                        USART1_RX_STA = 0; 
                }
            }
        }
 /*1*/  else if(temp == '#') 
        {
		int i;
		USART1_RX_STA |= 0x40;	//���USART1_RX_STA��7λ��1
		for(i = 0; i < 32; i++)	//�������		
		USART1_RX_BUF[i] = 0x00;
         }
 				
 /*6*/	 if((USART1_RX_STA&0x80)!=0)		//��������ȫ���͵���������������ת�ʹ���		
		{		
			int i;
			for(i=0;i<len_1;i++)
			{
				USART_SendData(USART1,USART1_RX_BUF[i]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
				
			}
			USART1_RX_STA=0;//������
			len_1 = 0;
		} 
		}
	
}      


