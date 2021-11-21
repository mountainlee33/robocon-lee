#include "param.h"

FlagTypeDef flag;
Usart_Struct usart;
ErrorTypeDef Error;
Usart_Struct usart;
System_Flag SystemFlag;
void Flag_Init(void)
{
    flag.Can2CtrlList_Enable=true;
}

void param_Init(void)
{
		Motor_Init();
		Flag_Init();
		LED_Configuration();
		Led8_Configuration();
}

/* 改变16位数据高低顺序 */
void ChangeData(u8 *buff2, u8 *buff1)
{
  u8 temp = 0;
  temp = *buff1;
  *buff1 = *buff2;
  *buff2 = temp;
}

//创建一个空错误链表
ErrorList *createError(void)
{
	ErrorList *head;
	head = (ErrorList *)malloc(sizeof(ErrorList));
	head->next = NULL;
	return head;
}

ErrorList *ErrorFind(ErrorList *list, uc16 errorcode)
{
	ErrorList *t = list;
	while (t != NULL)
	{
		if(t->code  == errorcode)
			return t;
		else
			t = t->next;
	}
	return NULL;
}

//删除错误
void deletError(ErrorList *list, ErrorList *listdel)
{
	ErrorList *list2 = list;
	while(list2 != NULL && (list2 -> next) != listdel)
	{
		list2 = list2 ->next;
	}
	if(listdel != NULL)
	{
		list2 -> next = listdel -> next;
		free(listdel);
		Error.size--;		
	}
}

//插入错误
void insertError(ErrorList *list, uc16 errorcode )
{
	ErrorList *t = list, *in;
	if(ErrorFind(t,errorcode) != NULL)
		return;
	u8 i =0;
	while(i < Error.size && t !=NULL)
	{
		t = t->next;
		i++;
	}
	if(t != NULL)
	{
		in = (ErrorList *)malloc (sizeof (ErrorList));
		in->code = errorcode;
		in->next = t->next;
		t->next = in;
		Error.size ++;
	}
}

