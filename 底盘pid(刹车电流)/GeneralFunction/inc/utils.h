#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include "stm32f4xx.h"

#define ABS(x)  ((x)>0? (x):(-(x)))
#define SIG(x)  ((x < 0) ? -1 : 1)
#define PEAK(A,B)   if(ABS(A)>B) A=SIG(A)*B; //此处默认B为正值
#define SQ(x)	((x)*(x))

// nan and infinity check for floats
#define UTILS_IS_INF(x)		((x) == (1.0f / 0.0f) || (x) == (-1.0f / 0.0f))
#define UTILS_IS_NAN(x)		((x) != (x))
#define UTILS_NAN_ZERO(x)	(x = UTILS_IS_NAN(x) ? 0.0f : x)

/**
 * A simple low pass filter.
 *
 * @param value
 * The filtered value.
 *
 * @param sample
 * Next sample.
 *
 * @param filter_constant
 * Filter constant. Range 0.0f to 1.0f, where 1.0f gives the unfiltered value.
 */
#define UTILS_LP_FAST(value, sample, filter_constant)	(value -= (filter_constant) * (value - (sample)))

// Some constants
#define ONE_BY_SQRT3			(0.57735026919f)
#define TWO_BY_SQRT3			(2.0f * 0.57735026919f)
#define SQRT3_BY_2				(0.86602540378f)
#define M_PI              (3.14159265358979323846f)

void utils_step_towards(float *value, float goal, float step);
float utils_calc_ratio(float low, float high, float val);
void utils_norm_angle(float *angle);
void utils_norm_angle_rad(float *angle);
int utils_truncate_number(float *number, float min, float max);
int utils_truncate_number_int(int *number, int min, int max);
int utils_truncate_number_abs(float *number, float max);
int utils_truncate_number_abs_s32(s32 *number, s32 max);
float utils_map(float x, float in_min, float in_max, float out_min, float out_max);
int utils_map_int(int x, int in_min, int in_max, int out_min, int out_max);
void utils_deadband(float *value, float tres, float max);
float utils_angle_difference(float angle1, float angle2);
float utils_angle_difference_rad(float angle1, float angle2);
float utils_avg_angles_rad_fast(float *angles, float *weights, int angles_num);
float utils_middle_of_3(float a, float b, float c);
int utils_middle_of_3_int(int a, int b, int c);
float utils_fast_inv_sqrt(float x);
float utils_fast_atan2(float y, float x);
bool utils_saturate_vector_2d(float *x, float *y, float max);
void utils_fast_sincos(float angle, float *sin, float *cos);
void utils_fast_sincos_better(float angle, float *sin, float *cos);
float utils_min_abs(float va, float vb);
float utils_max_abs(float va, float vb);
void utils_byte_to_binary(int x, char *b);
float utils_throttle_curve(float val, float curve_acc, float curve_brake, int mode);

#endif
