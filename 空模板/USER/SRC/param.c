#include "param.h"

void param_Init(void)
{
		LED_Configuration();
		Led8_Configuration();
		Beep_Init();
}

/* �ı�16λ���ݸߵ�˳�� */
void ChangeData(u8 *buff2, u8 *buff1)
{
  u8 temp = 0;
  temp = *buff1;
  *buff1 = *buff2;
  *buff2 = temp;
}

