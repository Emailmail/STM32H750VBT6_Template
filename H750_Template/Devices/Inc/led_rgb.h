#ifndef __LED_RGB_H
#define __LED_RGB_H
#include "stdlib.h"
#include "stdbool.h"
#include "stm32h750xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"

typedef struct
{
    GPIO_TypeDef *rport;
    uint8_t rpin;
    GPIO_TypeDef *gport;
    uint8_t gpin;
    GPIO_TypeDef *bport;
    uint8_t bpin;
    bool attr; // 0:LED低电平时熄灭; 1:LED高电平时熄灭
} LED_RGB_InitTypedef;

typedef struct
{
    GPIO_TypeDef *rport;
    uint8_t rpin;
    GPIO_TypeDef *gport;
    uint8_t gpin;
    GPIO_TypeDef *bport;
    uint8_t bpin;
    bool attr; // 0:LED低电平时熄灭; 1:LED高电平时熄灭
    bool state[3];
} LED_RGB_Instance;

LED_RGB_Instance *LED_RGB_Register(LED_RGB_InitTypedef *init);
void LED_RGB_Set(LED_RGB_Instance *instance, bool r, bool g, bool b);

#endif
