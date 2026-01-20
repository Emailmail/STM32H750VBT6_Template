/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task_control.h"
#include "stdio.h"
#include "usart.h"
#include "task_attitude.h"
#include "task_motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LightTask */
osThreadId_t LightTaskHandle;
const osThreadAttr_t LightTask_attributes = {
  .name = "LightTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for AttitudeTask */
osThreadId_t AttitudeTaskHandle;
const osThreadAttr_t AttitudeTask_attributes = {
  .name = "AttitudeTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh6,
};
/* Definitions for DisplayTask */
osThreadId_t DisplayTaskHandle;
const osThreadAttr_t DisplayTask_attributes = {
  .name = "DisplayTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh4,
};
/* Definitions for SpeedControlTas */
osThreadId_t SpeedControlTasHandle;
const osThreadAttr_t SpeedControlTas_attributes = {
  .name = "SpeedControlTas",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh5,
};
/* Definitions for AngleControlTas */
osThreadId_t AngleControlTasHandle;
const osThreadAttr_t AngleControlTas_attributes = {
  .name = "AngleControlTas",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh7,
};
/* Definitions for AngVelControlTa */
osThreadId_t AngVelControlTaHandle;
const osThreadAttr_t AngVelControlTa_attributes = {
  .name = "AngVelControlTa",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for RemoteTask */
osThreadId_t RemoteTaskHandle;
const osThreadAttr_t RemoteTask_attributes = {
  .name = "RemoteTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLightTask(void *argument);
void StartAttitudeTask(void *argument);
void StartDisplayTask(void *argument);
void StartMotorTask(void *argument);
void StartSpeedControlTask(void *argument);
void StartAngleControlTask(void *argument);
void StartAngVelControlTask(void *argument);
void StartRemoteTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LightTask */
  LightTaskHandle = osThreadNew(StartLightTask, NULL, &LightTask_attributes);

  /* creation of AttitudeTask */
  AttitudeTaskHandle = osThreadNew(StartAttitudeTask, NULL, &AttitudeTask_attributes);

  /* creation of DisplayTask */
  DisplayTaskHandle = osThreadNew(StartDisplayTask, NULL, &DisplayTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

  /* creation of SpeedControlTas */
  SpeedControlTasHandle = osThreadNew(StartSpeedControlTask, NULL, &SpeedControlTas_attributes);

  /* creation of AngleControlTas */
  AngleControlTasHandle = osThreadNew(StartAngleControlTask, NULL, &AngleControlTas_attributes);

  /* creation of AngVelControlTa */
  AngVelControlTaHandle = osThreadNew(StartAngVelControlTask, NULL, &AngVelControlTa_attributes);

  /* creation of RemoteTask */
  RemoteTaskHandle = osThreadNew(StartRemoteTask, NULL, &RemoteTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  
  /* Infinite loop */
  for(;;)
  {
    printf("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r\n", turn_pid->p_out, turn_pid->i_out, turn_pid->d_out, turn_pid->output, target_speed, deltaspeed);
    // printf("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r\n", speed_pid->p_out, speed_pid->i_out, speed_pid->d_out, speed_pid->output, speed, target_speed);
    // printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", angle_pid->p_out, angle_pid->i_out, angle_pid->d_out, angle_pid->output, pitch, target_speed);
    // printf("%.2f,%.2f,%.2f,%.2f,%.2f\r\n", angvel_pid->p_out, angvel_pid->i_out, angvel_pid->d_out, angvel_pid->output, lsm6dsv->gyro[0]);
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLightTask */
/**
* @brief Function implementing the LightTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLightTask */
__weak void StartLightTask(void *argument)
{
  /* USER CODE BEGIN StartLightTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartLightTask */
}

/* USER CODE BEGIN Header_StartAttitudeTask */
/**
* @brief Function implementing the AttitudeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAttitudeTask */
__weak void StartAttitudeTask(void *argument)
{
  /* USER CODE BEGIN StartAttitudeTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartAttitudeTask */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
* @brief Function implementing the DisplayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayTask */
__weak void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN StartDisplayTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
__weak void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartSpeedControlTask */
/**
* @brief Function implementing the SpeedControlTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSpeedControlTask */
__weak void StartSpeedControlTask(void *argument)
{
  /* USER CODE BEGIN StartSpeedControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSpeedControlTask */
}

/* USER CODE BEGIN Header_StartAngleControlTask */
/**
* @brief Function implementing the AngleControlTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAngleControlTask */
__weak void StartAngleControlTask(void *argument)
{
  /* USER CODE BEGIN StartAngleControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartAngleControlTask */
}

/* USER CODE BEGIN Header_StartAngVelControlTask */
/**
* @brief Function implementing the AngVelControlTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAngVelControlTask */
__weak void StartAngVelControlTask(void *argument)
{
  /* USER CODE BEGIN StartAngVelControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartAngVelControlTask */
}

/* USER CODE BEGIN Header_StartRemoteTask */
/**
* @brief Function implementing the RemoteTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRemoteTask */
__weak void StartRemoteTask(void *argument)
{
  /* USER CODE BEGIN StartRemoteTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRemoteTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/* USER CODE END Application */

