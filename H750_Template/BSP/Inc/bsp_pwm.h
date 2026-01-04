#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#define BSP_PWM_CALLBACK (0)

#include "tim.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h750xx.h"

#define PWM_DEVICE_CNT 16 // 最大支持的PWM实例数量

/* pwm实例结构体 */
typedef struct pwm_struct
{
    TIM_HandleTypeDef *htim;               // TIM句柄
    uint32_t channel;                      // 通道
    uint32_t tclk;                         // 时钟频率
    float period;                          // 周期
    float dutyratio;                       // 占空比
#if (BSP_PWM_CALLBACK)
    void (*callback)(struct pwm_struct *); // DMA传输完成回调函数
    void *id;                              // 实例ID
#endif
} PWM_Instance;

typedef struct
{
    TIM_HandleTypeDef *htim; // TIM句柄
    uint32_t channel;        // 通道
    float period;            // 周期
    float dutyratio;         // 占空比
#if (BSP_PWM_CALLBACK)
    void (*callback)(PWM_Instance *); // DMA传输完成回调函数
    void *id;                         // 实例ID
#endif
} PWM_Init_Config_s;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
PWM_Instance *PWM_Register(PWM_Init_Config_s *config);
void PWM_Start(PWM_Instance *pwm);
void PWM_Stop(PWM_Instance *pwm);
void PWM_SetPeriod(PWM_Instance *pwm, float period);
void PWM_SetDutyRatio(PWM_Instance *pwm, float dutyratio);
void PWM_StartDMA(PWM_Instance *pwm, uint32_t *pData, uint32_t Size);

#endif
