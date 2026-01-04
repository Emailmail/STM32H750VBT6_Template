#include "lowpass_filter.h"

/**
 * @brief 初始化低通滤波器
 * @param filter 滤波器结构体指针
 * @param alpha 滤波系数
 */
uint8_t LowPassFilter_Init(LowPass_Filter *filter, float alpha)
{
    if(alpha <= 0.0f || alpha > 1.0f)
    {
        return 1;
    }

    filter->alpha = alpha;
    filter->value = 0.0f;
    filter->value_prev = 0.0f;

    return 0;
}

/**
 * @brief 更新低通滤波器
 * @param filter 滤波器结构体指针
 * @param input 输入数据
 */
float LowPassFilter_GetValue(LowPass_Filter *filter, float input)
{
    filter->value = filter->alpha * input + (1.0f - filter->alpha) * filter->value_prev;
    filter->value_prev = filter->value;
    return filter->value;
}
