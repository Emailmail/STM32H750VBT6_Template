#include "task_control.h"
#include "task_attitude.h"
#include "task_motor.h"

/* PID instances */
PID_Controller *speed_pid;
PID_Controller *angle_pid;
PID_Controller *angvel_pid;

/* Middleware variables */
float target_angvel = 0.0f;
float target_angle = 0.0f;
float target_speed = 0.0f;
float speed = 0.0f;

/**
 * @brief 速度闭环控制任务
 */
void StartSpeedControlTask(void *argument)
{
    TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(100);
    vTaskDelay(pdMS_TO_TICKS(100));

    PID_Init_Config_s speed_pid_config =
        {
            .Kp = 0.5f,
            .Ki = 0.00f,
            .Kd = 3.0f,
        };
    speed_pid = PID_Register(&speed_pid_config);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        speed = (tb6612_motor1->speed + tb6612_motor2->speed) / 2.0f;
        target_angle = - PID_Calculate(speed_pid, target_speed - speed);

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief 角位置闭环控制任务
 */
void StartAngleControlTask(void *argument)
{
    TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(20);
    vTaskDelay(pdMS_TO_TICKS(100));

    PID_Init_Config_s angle_pid_config =
        {
            .Kp = 0.3500f,
            .Ki = 0.005f,
            .Kd = 0.5f,
        };
    angle_pid = PID_Register(&angle_pid_config);
    PID_SetIntegralLimit(angle_pid, -1000.0f, 1000.0f);
    PID_SetIntegralSeperate(angle_pid, -10.0f, 10.0f);
    PID_SetLowPassFilter(angle_pid, 0.1f);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        target_angvel = PID_Calculate(angle_pid, target_angle - pitch);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief 角速度闭环控制任务
 */
void StartAngVelControlTask(void *argument)
{
    TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(5);
    vTaskDelay(pdMS_TO_TICKS(100));

    PID_Init_Config_s angvel_pid_config =
        {
            .Kp = 0.0800f,
            .Ki = 0.0000f,
            .Kd = 0.0000f,
        };
    angvel_pid = PID_Register(&angvel_pid_config);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        duty = PID_Calculate(angvel_pid, target_angvel - lsm6dsv->gyro[0] + GYROX_OFFSET);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
