#include "numchange.h"
//����ת��

//��float���͵����ݷŵ��ĸ��ֽ���
void EcodeFloatData_to_4byte(float *f, u8 *buff)
{
	u16 f_c[2];
	*(float *)f_c = *f;
	buff[0] = (f_c[0] >> 0 & 0x00ff);
	buff[1] = ((f_c[0] >> 8) & 0x00ff);
	buff[2] = (f_c[1] >> 0 & 0x00ff);
	buff[3] = ((f_c[1] >> 8) & 0x00ff);
}

//��number�ŵ�����buffer�У�number�ĸ�λ��Ӧbuffer�ĵ��ֽ�
void buffer_append_int32(uint8_t *buffer, int32_t number, int32_t *index)
{
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

//���ĸ��ֽ�u8���͵����ݺϳ�Ϊint���ͷ���
s32 get_s32_from_buffer(const u8 *buffer, s32 *index)
{
	s32 res = ((uint32_t)buffer[*index]) << 24 |
			  ((uint32_t)buffer[*index + 1]) << 16 |
			  ((uint32_t)buffer[*index + 2]) << 8 |
			  ((uint32_t)buffer[*index + 3]);
	*index += 4;
	return res;
}

//�������ֽ�u8���͵����ݺϳ�Ϊint���ͷ���
s16 get_s16_from_buffer(const u8 *buffer, s32 *index)
{
	s16 res = ((uint32_t)buffer[*index]) << 8 |
			  ((uint32_t)buffer[*index + 1]);
	*index += 2;
	return res;
}

//�������ֽ�u8���͵����ݺϳ�Ϊfloat���ͳ���scale֮�󷵻�
float buffer_16_to_float(const uint8_t *buffer, float scale, int32_t *index)
{
	return (float)get_s16_from_buffer(buffer, index) / scale;
}
