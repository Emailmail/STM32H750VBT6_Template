/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define ST7789_BLK_Pin GPIO_PIN_0
#define ST7789_BLK_GPIO_Port GPIOC
#define ST7789_CS_Pin GPIO_PIN_1
#define ST7789_CS_GPIO_Port GPIOC
#define ST7789_NC_Pin GPIO_PIN_2
#define ST7789_NC_GPIO_Port GPIOC
#define ST7789_SDA_Pin GPIO_PIN_3
#define ST7789_SDA_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define TB6612_E1A_Pin GPIO_PIN_1
#define TB6612_E1A_GPIO_Port GPIOA
#define IBUS_TX_Pin GPIO_PIN_2
#define IBUS_TX_GPIO_Port GPIOA
#define IBUS_RX_Pin GPIO_PIN_3
#define IBUS_RX_GPIO_Port GPIOA
#define TB6612_E1B_Pin GPIO_PIN_5
#define TB6612_E1B_GPIO_Port GPIOA
#define TB6612_E2A_Pin GPIO_PIN_6
#define TB6612_E2A_GPIO_Port GPIOA
#define TB6612_E2B_Pin GPIO_PIN_7
#define TB6612_E2B_GPIO_Port GPIOA
#define TB6612_STBY_Pin GPIO_PIN_4
#define TB6612_STBY_GPIO_Port GPIOC
#define TB6612_BIN1_Pin GPIO_PIN_12
#define TB6612_BIN1_GPIO_Port GPIOB
#define TB6612_BIN2_Pin GPIO_PIN_13
#define TB6612_BIN2_GPIO_Port GPIOB
#define TB6612_PWMB_Pin GPIO_PIN_14
#define TB6612_PWMB_GPIO_Port GPIOB
#define TB6612_PWMA_Pin GPIO_PIN_15
#define TB6612_PWMA_GPIO_Port GPIOB
#define TB6612_AIN1_Pin GPIO_PIN_6
#define TB6612_AIN1_GPIO_Port GPIOC
#define TB6612_AIN2_Pin GPIO_PIN_7
#define TB6612_AIN2_GPIO_Port GPIOC
#define VOFA_TX_Pin GPIO_PIN_9
#define VOFA_TX_GPIO_Port GPIOA
#define VOFA_RX_Pin GPIO_PIN_10
#define VOFA_RX_GPIO_Port GPIOA
#define ST7789_SCL_Pin GPIO_PIN_12
#define ST7789_SCL_GPIO_Port GPIOA
#define DAP_SWD_Pin GPIO_PIN_13
#define DAP_SWD_GPIO_Port GPIOA
#define DAP_SCK_Pin GPIO_PIN_14
#define DAP_SCK_GPIO_Port GPIOA
#define KEY0_Pin GPIO_PIN_15
#define KEY0_GPIO_Port GPIOA
#define KEY0_EXTI_IRQn EXTI15_10_IRQn
#define ST7789_RES_Pin GPIO_PIN_11
#define ST7789_RES_GPIO_Port GPIOC
#define ST7789_DC_Pin GPIO_PIN_3
#define ST7789_DC_GPIO_Port GPIOD
#define LSM6DSV_SA0_Pin GPIO_PIN_4
#define LSM6DSV_SA0_GPIO_Port GPIOB
#define LSM6DSV_INT_Pin GPIO_PIN_7
#define LSM6DSV_INT_GPIO_Port GPIOB
#define LSM6DSV_SCL_Pin GPIO_PIN_8
#define LSM6DSV_SCL_GPIO_Port GPIOB
#define LSM6DSV_SDA_Pin GPIO_PIN_9
#define LSM6DSV_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
