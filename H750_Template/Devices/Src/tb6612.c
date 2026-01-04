#include "tb6612.h"

TB6612_Instance *TB6612_Register(TB6612_InitTypedef *init) 
{ 
    if(init == NULL)
        return NULL;
    if(init->pwm_init == NULL || init->in1_port == NULL || init->in2_port == NULL || init->hencoder == NULL)
        return NULL;

    TB6612_Instance *instance = malloc(sizeof(TB6612_Instance));
    if(instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(TB6612_Instance));

    instance->hpwm = PWM_Register(init->pwm_init);
    if(instance->hpwm == NULL)
        return NULL;

    instance->in1_port = init->in1_port;
    instance->in1_pin = init->in1_pin;
    instance->in2_port = init->in2_port;
    instance->in2_pin = init->in2_pin;
    instance->hencoder = init->hencoder;
    instance->channelA = init->channelA;
    instance->channelB = init->channelB;
    instance->ppr = init->ppr;
    instance->reduction_ratio = init->reduction_ratio;

    return instance;
}

void TB6612_SetDuty(TB6612_Instance *instance, float duty)
{
    /* Set In1 and In2 to change the direction */
    if(duty >= 0)
    {
        HAL_GPIO_WritePin(instance->in1_port, instance->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(instance->in2_port, instance->in2_pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(instance->in1_port, instance->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(instance->in2_port, instance->in2_pin, GPIO_PIN_SET);
    }

    /* Set PWM duty ratio */
    PWM_SetDutyRatio(instance->hpwm, fabsf(duty));
}

void TB6612_GetSpeed_Start(TB6612_Instance *instance)
{
    HAL_TIM_Encoder_Start(instance->hencoder, instance->channelA | instance->channelB);
    instance->last_tick = HAL_GetTick();
    __HAL_TIM_SET_COUNTER(instance->hencoder, 0);
}

float TB6612_GetSpeed(TB6612_Instance *instance)
{
    uint32_t count = __HAL_TIM_GET_COUNTER(instance->hencoder);
    uint32_t current_tick = HAL_GetTick();
    uint32_t delta_tick = current_tick - instance->last_tick;

    instance->speed = (float)count / delta_tick / instance->ppr / instance->reduction_ratio * 1000.0f;
    instance->last_tick = current_tick;
    __HAL_TIM_SET_COUNTER(instance->hencoder, 0);
    return instance->speed;
}
