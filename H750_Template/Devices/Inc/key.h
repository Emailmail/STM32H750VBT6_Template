#ifndef __KEY_H
#define __KEY_H
#include "stdlib.h"
#include "stdbool.h"
#include "stm32h750xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"

#define DELAY_TIME 50 // 消抖延时的时间,单位ms

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool attr;  // 0:松开时为低电平;1:松开时为高电平
    uint8_t dt; // 按键检测间隔
} Key_InitTypedef;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool attr;           // 0:松开时为低电平;1:松开时为高电平
    uint8_t delay_count; // 消抖时需要的延时次数,至少为1
    uint8_t count;       // 当前的按键检测"状态"
    uint8_t timer;       // 计数器
    bool state;          // 0:松开;1:按下
} Key_Instance;

Key_Instance *Key_Register(Key_InitTypedef *init);
bool Key_Detect(Key_Instance *instance);

#endif