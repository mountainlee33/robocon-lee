#include "HX711.h"

u32 HX711_Buffer;
u32 Weight_Peel;
u32 delta;
u32 Weight_Shiwu;
u8 Flag_Error = 0;

//У׼����
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
//�ֶ�����
#define GapValue 252.5

void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//ʹ��PA�˿�ʱ��

	//HX711_SCK,PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				// �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		 	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB
	
	//HX711_DOUT����������out�����ǵ�in,
	//PA1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��Ƭ����ʵ��ȡ����HX711ת���������������Ӧ����ģ������
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);							//��ʼ������Ϊ0
}

//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; //���F4����������Ϊ1
	Delay_us(1);
  	HX711_SCK=0; //��SCK����͵�ƽ��
  	count=0; 
  	while(HX711_DOUT); //��ȡ��0��ʼ���м���
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		Delay_us(1);
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
		Delay_us(1);
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//��25�������½�����ʱ��ת������
	Delay_us(1);
	HX711_SCK=0;  
	return(count);
}


//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Peel(void)
{
	Weight_Peel = HX711_Read();	
} 

//��һ�ζ�����ֵ��Ϊ��ëƤ��������whileѭ���ж�����ʵ��
void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Peel)			
	{
		delta = HX711_Buffer - Weight_Peel;				//��ȡʵ���AD������ֵ��


    Weight_Shiwu=(u32)(delta/GapValue);
	}
	
}

