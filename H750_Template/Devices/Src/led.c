#include "led.h"

/**
 * @brief 注册LED实例
 */
LED_Instance *LED_Register(LED_InitTypedef *init)
{
    /* 检验参数合法性 */
    if(init == NULL)
        return NULL;

    /* 分配实例结构体内存 */
    LED_Instance *instance = (LED_Instance *)malloc(sizeof(LED_Instance));
    if(instance == NULL)
        return NULL;

    /* 参数传递 */
    instance->port = init->port;
    instance->pin = init->pin;
    instance->attr = init->attr;
    instance->state = 0;

    /* 默认熄灭改LED */
    LED_Off(instance);

    /* 返回指针 */
    return instance;
}

/**
 * @brief LED亮
 */
void LED_On(LED_Instance *instance)
{
    HAL_GPIO_WritePin(instance->port, instance->pin, !instance->attr);
    instance->state = 1;
}

/**
 * @brief LED关
 */
void LED_Off(LED_Instance *instance)
{
    HAL_GPIO_WritePin(instance->port, instance->pin, instance->attr);
    instance->state = 0;
}

/**
 * @brief 控制LED
 */
void LED_Control(LED_Instance *instance, bool state)
{
    HAL_GPIO_WritePin(instance->port, instance->pin, state^instance->attr);
    instance->state = state;
}
