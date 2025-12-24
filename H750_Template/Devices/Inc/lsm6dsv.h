#ifndef _LSM6DSV_H_
#define _LSM6DSV_H_
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stm32h750xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_i2c.h"
#include "lsm6dsv16x_reg.h"

#define LSM6DSV_NUM_MAX 4

typedef struct
{
    I2C_HandleTypeDef *hi2c;
    GPIO_TypeDef *sa0_port;
    uint16_t sa0_pin;
    GPIO_TypeDef *int_port;
    uint16_t int_pin;
} LSM6DSV_InitTypedef;

typedef struct
{
    I2C_HandleTypeDef *hi2c;
    GPIO_TypeDef *sa0_port;
    uint16_t sa0_pin;
    GPIO_TypeDef *int_port;
    uint16_t int_pin;

    uint8_t device_id;
    lsm6dsv16x_data_ready_t drdy;

    int16_t data_raw_acceleration[3];
    int16_t data_raw_angular_rate[3];
    int16_t data_raw_temperature;

    float accel[3];
    float gyro[3];
    float temperature;
} LSM6DSV_Instance;


#endif


