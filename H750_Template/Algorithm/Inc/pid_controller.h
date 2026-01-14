#ifndef _PID_H_
#define _PID_H_
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "lowpass_filter.h"
#include "math.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

typedef struct
{
    /* 基本参数 */
    float Kp;
    float Ki;
    float Kd;

    /* 时间戳 */
    uint32_t last_tick;

    /* 微分项滤波设置 */
    bool USE_LOWPASS_FILTER;
    LowPass_Filter diff_lowpassfilter;

    /* 积分分离设置 */
    bool USE_INTEGRAL_SEPARATE;
    float input_integral_max;
    float input_integral_min;

    /* 变速积分 */
    bool USE_CHANGE_INTEGRAL;
    float error_integral_threshold;

    /* 积分限幅设置 */
    bool USE_INTEGRAL_LIMIT;
    float integral_max;
    float integral_min;

    /* 输出限幅设置 */
    bool USE_OUTPUT_LIMIT;
    float output_max;
    float output_min;

    /* 中间变量 */
    float error;
    float last_error;
    float last_last_error;
    float integral_error;
    float rate_error;

    /* 输出 */
    float p_out;
    float i_out;
    float d_out;
    float output;
} PID_Controller;

typedef struct 
{
    float Kp;
    float Ki;
    float Kd;
} PID_Init_Config_s;


PID_Controller *PID_Register(PID_Init_Config_s *init_config);
uint8_t PID_SetLowPassFilter(PID_Controller *pid, float alpha);
uint8_t PID_SetChangeIntegral(PID_Controller *pid, float error_integral_threshold);
uint8_t PID_SetIntegralSeperate(PID_Controller *pid, float integral_min, float integral_max);
uint8_t PID_SetIntegralLimit(PID_Controller *pid, float integral_min, float integral_max);
uint8_t PID_SetOutputLimit(PID_Controller *pid, float output_min, float output_max);
float PID_Calculate(PID_Controller *pid, float error);
float PID_Increment(PID_Controller *pid, float error);

#endif
