#include "task_control.h"

/* PID 实例 */
PID_Controller *speed_pid;  // 速度环PID
PID_Controller *angle_pid;  // 角度环PID
PID_Controller *angvel_pid; // 角速度环PID
PID_Controller *turn_pid;   // 转向环PID

LowPass_Filter gyrox_lowpass;

/* 中间变量 */
float target_angvel = 0.0f;      // 角速度目标值
float target_angle = 0.0f;       // 角度目标值
float target_speed = 0.0f;       // 速度目标值
float target_turn = 0.0f;        // 差速目标值
float target_deltaspeed = 0.0f; // 差速目标值

/**
 * @brief 速度闭环控制任务
 */
void StartSpeedControlTask(void *argument)
{
    TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(100);
    vTaskDelay(pdMS_TO_TICKS(100));

    /* PID注册,速度环PID,转向环PID */
    PID_Init_Config_s speed_pid_config =
        {
            .Kp = 1.8000f,
            .Ki = 0.0000f,
            .Kd = 0.0000f,
        };
    speed_pid = PID_Register(&speed_pid_config);
    PID_Init_Config_s turn_pid_config =
        {
            .Kp = 0.055f,
            .Ki = 0.00001f,
            .Kd = 0.0000f,
        };
    turn_pid = PID_Register(&turn_pid_config);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        /* 读取用户利用X6B接收机输入的目标速度 */
        target_speed = -left_y * 5.0f;
        target_deltaspeed = right_x * 2.0f;

        /* 速度、差速闭环控制实现 */
        if (control_flag == 0)  // 未使能控制
        {
            target_angle = 0.0f;
        }
        else    // 使能控制
        {
            /* 转向环控制实现 */
            delta_dutyleft = PID_Calculate(turn_pid, target_deltaspeed - deltaspeed);
            delta_dutyright = -delta_dutyleft;

            /* 速度环控制实现 */
            target_angle = PID_Calculate(speed_pid, target_speed - speed);
        }
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

    /* 注册角位置环PID */
    PID_Init_Config_s angle_pid_config =
        {
            .Kp = 0.6000f,
            .Ki = 0.0015f,
            .Kd = 14.0000f,
        };
    angle_pid = PID_Register(&angle_pid_config);
    PID_SetIntegralLimit(angle_pid, -300.0f, 300.0f);
    PID_SetIntegralSeperate(angle_pid, -5.0f, 5.0f);
    PID_SetLowPassFilter(angle_pid, 0.259f);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        /* 角位置环控制实现 */
        if (control_flag == 0)  // 未使能控制
        {
            target_angvel = 0.0f;
        }
        else    // 使能控制
        {
            /* 角位置环控制实现 */
            target_angvel = PID_Calculate(angle_pid, target_angle - pitch);
        }
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

    /* 注册角速度环PID */
    PID_Init_Config_s angvel_pid_config =
        {
            .Kp = 0.1400f,
            .Ki = 0.0000f,
            .Kd = 0.0000f,
        };
    angvel_pid = PID_Register(&angvel_pid_config);
    LowPassFilter_Init(&gyrox_lowpass, 0.759f);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    for (;;)
    {
        /* 角速度环控制实现 */
        if (control_flag == 0)  // 未使能控制
        {
            duty = 0.0f;
        }
        else    // 使能控制
        {
            /* 角速度环控制实现 */
            duty = -PID_Calculate(angvel_pid, target_angvel - LowPassFilter_GetValue(&gyrox_lowpass, lsm6dsv->gyro[0]) + GYROX_OFFSET);
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
