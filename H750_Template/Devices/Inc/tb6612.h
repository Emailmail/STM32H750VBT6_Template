#ifndef __TB6612_H
#define __TB6612_H
#include "math.h"
#include "bsp_pwm.h"
typedef struct
{
    /* Output IOs */
    PWM_Init_Config_s *pwm_init;
    GPIO_TypeDef *in1_port;
    uint16_t in1_pin;
    GPIO_TypeDef *in2_port;
    uint16_t in2_pin;

    /* Input IOs */
    TIM_HandleTypeDef *hencoder;
    uint32_t channelA;
    uint32_t channelB;

    /* Motor Params */
    uint32_t ppr;             // Pulse Per Rotation
    uint32_t reduction_ratio; // Reduction Ratio

} TB6612_InitTypedef;

typedef struct
{
    /* Output IOs */
    PWM_Instance *hpwm;
    GPIO_TypeDef *in1_port;
    uint16_t in1_pin;
    GPIO_TypeDef *in2_port;
    uint16_t in2_pin;

    /* Input IOs */
    TIM_HandleTypeDef *hencoder;
    uint32_t channelA;
    uint32_t channelB;

    /* Motor Params */
    uint32_t ppr;             // Pulse Per Rotation
    uint32_t reduction_ratio; // Reduction Ratio

    uint32_t last_tick;
    float speed;
} TB6612_Instance;

TB6612_Instance *TB6612_Register(TB6612_InitTypedef *init);
void TB6612_SetDuty(TB6612_Instance *instance, float duty);
void TB6612_GetSpeed_Start(TB6612_Instance *instance);
float TB6612_GetSpeed(TB6612_Instance *instance);
 
#endif
