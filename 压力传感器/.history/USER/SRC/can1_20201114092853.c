#include "can1.h"

void CAN1_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef Can_FilterInitStrucutre;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
   
	GPIO_Set(GPIOA,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_25M,GPIO_PUPD_NONE);
    GPIO_AF_Set(GPIOA,11,9);    //CAN1_RX PA11        
    GPIO_AF_Set(GPIOA,12,9);	//CAN1_TX PA12

//GPIO��������GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
//BITx:0~15,����IO���ű�ţ�AF9��CAN1/CAN2/TIM12~14
  
	/* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE; //��ʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM = DISABLE; //�����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
    CAN_InitStructure.CAN_AWUM = DISABLE; //˯��ģʽ���������
    CAN_InitStructure.CAN_NART = DISABLE; //��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
    CAN_InitStructure.CAN_RFLM = DISABLE; //���Ĳ��������µĸ��Ǿɵ�
    CAN_InitStructure.CAN_TXFP = DISABLE; //����FIFO�����ȼ��ɱ�ʶ������
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CANӲ������������ģʽ

    /* Seting BaudRate */
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //����ͬ����Ծ���Ϊһ��ʱ�䵥λ
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //ʱ���1ռ��8��ʱ�䵥λ
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //ʱ���2ռ��7��ʱ�䵥λ
    CAN_InitStructure.CAN_Prescaler = 3;     //��Ƶϵ����Fdiv��
    CAN_Init(CAN1, &CAN_InitStructure);      //��ʼ��CAN1
	//�����ʣ�42M/((9+1+4)*3)=1Mbps

    /* CAN filter Init */
	Can_FilterInitStrucutre.CAN_FilterNumber=0;//����
	Can_FilterInitStrucutre.CAN_FilterMode=CAN_FilterMode_IdMask;
	Can_FilterInitStrucutre.CAN_FilterScale=CAN_FilterScale_32bit;
	Can_FilterInitStrucutre.CAN_FilterIdHigh=((ID_SELF<<3)&0xffff0000)>>16;
	Can_FilterInitStrucutre.CAN_FilterIdLow=(ID_SELF<<3)&0xffff;
	Can_FilterInitStrucutre.CAN_FilterMaskIdHigh=0x00;//(0xffff00<<3)>>16;
	Can_FilterInitStrucutre.CAN_FilterMaskIdLow=0x00;//(0xffff00<<3)&0xffff;
    Can_FilterInitStrucutre.CAN_FilterActivation=ENABLE;				
	Can_FilterInitStrucutre.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInit(&Can_FilterInitStrucutre);

    MY_NVIC_Init(1,1,CAN1_RX0_IRQn,3);//�����ж����ȼ��Ȳ���
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        answer_master(&rx_message);
    }
}

//���ط���ָ��,������/������
static void answer_master(const CanRxMsg *rx_message)
{	
	CanTxMsg tx_message;
	tx_message.ExtId = ID_BACK;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = rx_message->DLC;
	tx_message.Data[0] = rx_message->Data[0]+0x40;
	tx_message.Data[1] = rx_message->Data[1];
	tx_message.Data[2] = rx_message->Data[2];
	tx_message.Data[3] = rx_message->Data[3];
	tx_message.Data[4] = rx_message->Data[4];
	tx_message.Data[5] = rx_message->Data[5];
	tx_message.Data[6] = rx_message->Data[6];
	tx_message.Data[7] = rx_message->Data[7];
	CAN_Transmit(CAN1,&tx_message);	
}


void Can1_BusTest(void)
{
	
	CanTxMsg TxMessage;
	TxMessage.ExtId=0X00020101;
	TxMessage.RTR=CAN_RTR_Data;	//��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.IDE=CAN_Id_Extended;//ʹ����չ��ʶ��
	TxMessage.DLC=3;
	TxMessage.Data[0]='W';
	TxMessage.Data[1]='H';
	TxMessage.Data[2]='U';
	
	CAN_Transmit(CAN1,&TxMessage);	
}
