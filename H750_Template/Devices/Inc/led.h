#ifndef __LED_H
#define __LED_H
#include "stdlib.h"
#include "stdbool.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"

/**
 * @brief 注册LED实例时的初始化结构体
 */
typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool attr;  // 0:LED低电平时熄灭; 1:LED高电平时熄灭
} LED_InitTypedef;

/**
 * @brief LED实例结构体类型
 */
typedef struct
{
	GPIO_TypeDef *port;
    uint16_t pin;
    bool attr;  // 0:LED低电平时熄灭; 1:LED高电平时熄灭
    bool state; // 0:LED熄灭; 1:LED亮起
} LED_Instance;

LED_Instance *LED_Register(LED_InitTypedef *init);
void LED_On(LED_Instance *instance);
void LED_Off(LED_Instance *instance);
void LED_Control(LED_Instance *instance, bool state);
#endif
