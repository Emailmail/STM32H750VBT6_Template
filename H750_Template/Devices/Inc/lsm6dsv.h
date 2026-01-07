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
#define MDPS_TO_RAD_PER_SEC  0.000017453292519943295f

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

    float accel[3]; // g
    float gyro[3];  // dps
    float temperature;
} LSM6DSV_Instance;

LSM6DSV_Instance *LSM6DSV_Register(LSM6DSV_InitTypedef *init);
int16_t LSM6DSV_Init(LSM6DSV_Instance *instance);
int32_t LSM6DSV_IsBusy(LSM6DSV_Instance *instance);
void LSM6DSV_ReadAccel(LSM6DSV_Instance *instance);
void LSM6DSV_ReadGyro(LSM6DSV_Instance *instance);
void LSM6DSV_ReadTemperature(LSM6DSV_Instance *instance);
uint8_t LSM6DSV_ReadData(LSM6DSV_Instance *instance);
#endif


