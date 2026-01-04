#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H
#include "stdint.h"

typedef struct  // 低通滤波器实例
{
    float alpha;
    float value;
    float value_prev;
} LowPass_Filter;

uint8_t LowPassFilter_Init(LowPass_Filter *filter, float alpha);
float LowPassFilter_GetValue(LowPass_Filter *filter, float input);
#endif
