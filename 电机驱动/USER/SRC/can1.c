#include  "can1.h"

void CAN1_Mode_Init(void)
{
	CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
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
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //ʱ���2ռ��3��ʱ�䵥λ
  CAN_InitStructure.CAN_Prescaler = 3;     //��Ƶϵ����Fdiv��
  CAN_Init(CAN1, &CAN_InitStructure); 	//��ʼ��CAN1
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
	
	
	/* �����ʼ��㹫ʽ: BaudRate = APB1ʱ��Ƶ��/Fdiv/��SJW+BS1+BS2�� */
  /* 42MHz/3/(1+9+4)=1Mhz */
	
	CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
}
void CAN1_RX0_IRQHandler(void)
{
	
}
	