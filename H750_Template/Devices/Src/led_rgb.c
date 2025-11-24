#include "led_rgb.h"

/**
 * @brief 注册LED
 */
LED_RGB_Instance *LED_RGB_Register(LED_RGB_InitTypedef *init)
{
    if(init == NULL)
        return NULL;

    LED_RGB_Instance *instance = malloc(sizeof(LED_RGB_Instance));
    if(instance == NULL)
        return NULL;

    instance->rport = init->rport;
    instance->rpin = init->rpin;
    instance->gport = init->gport;
    instance->gpin = init->gpin;
    instance->bport = init->bport;
    instance->bpin = init->bpin;
    instance->attr = init->attr;
    instance->state[0] = false;
    instance->state[1] = false;
    instance->state[2] = false;

    LED_RGB_Set(instance, false, false, false);

    return instance;
}

/**
 * @brief 设置LED
 */
void LED_RGB_Set(LED_RGB_Instance *instance, bool r, bool g, bool b)
{
    if(instance == NULL)
        return;

    HAL_GPIO_WritePin(instance->rport, instance->rpin, r ^ instance->attr);
    HAL_GPIO_WritePin(instance->gport, instance->gpin, g ^ instance->attr);
    HAL_GPIO_WritePin(instance->bport, instance->bpin, b ^ instance->attr);

    instance->state[0] = r;
    instance->state[1] = g;
    instance->state[2] = b;
}