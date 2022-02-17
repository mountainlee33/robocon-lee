#ifndef _MACRO_H_
#define _MACRO_H_

#include "stdbool.h"
#include "stm32f4xx.h"

#define USE_DJ 1
#define USE_ELMO 1
#define USE_VESC 1
#define USE_VALVE 1
#define USE_ODRIVE 1
#define USE_AK 1

/* BIT编号，用于流程号判断 */
#define BIT0 (0x01 >> 0)
#define BIT1 (0x01 >> 1)
#define BIT2 (0x01 >> 2)
#define BIT3 (0x01 >> 3)
#define BIT4 (0x01 >> 4)
#define BIT5 (0x01 >> 5)
#define BIT6 (0x01 >> 6)
#define BIT7 (0x01 >> 7)

#define EncodeS32Data(f, buff) \
	{                          \
		*(s32 *)buff = *f;     \
	}
#define DecodeS32Data(f, buff) \
	{                          \
		*f = *(s32 *)buff;     \
	}
#define EncodeU32Data(f, buff) \
	{                          \
		*(u32 *)buff = *f;     \
	}
#define DecodeU32Data(f, buff) \
	{                          \
		*f = *(u32 *)buff;     \
	}
#define EncodeS16Data(f, buff) \
	{                          \
		*(s16 *)buff = *f;     \
	}
#define DecodeS16Data(f, buff) \
	{                          \
		*f = *(s16 *)buff;     \
	}
#define EncodeU16Data(f, buff) \
	{                          \
		*(u16 *)buff = *f;     \
	}
#define DecodeU16Data(f, buff) \
	{                          \
		*f = *(u16 *)buff;     \
	}
	
#define PI 3.141592653
#endif
