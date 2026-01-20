#include "pid_controller.h"

/**
 * @brief PID实例注册
 * @param init_config PID初始化配置结构体指针
 * @retval PID实例指针
 */
PID_Controller *PID_Register(PID_Init_Config_s *init_config)
{
    if(init_config == NULL)
        return NULL;
    
    PID_Controller *instance = (PID_Controller *)pvPortMalloc(sizeof(PID_Controller));
    if(instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(PID_Controller));

    instance->Kd = init_config->Kd;
    instance->Ki = init_config->Ki;
    instance->Kp = init_config->Kp;
    instance->last_tick = HAL_GetTick();

    return instance;
}

/**
 * @brief 设置死区
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param dead_zone 死区值
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetDeadZone(PID_Controller *pid, float dead_zone)
{
    if(pid == NULL || dead_zone < 0.0f)
        return 1;

    pid->dead_zone = dead_zone;
    pid->USE_DEAD_ZONE = 1;

    return 0;
}

/**
 * @brief 设置微分项低通滤波
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param alpha 滤波系数，范围在0到1之间。
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetLowPassFilter(PID_Controller *pid, float alpha)
{
    if(pid == NULL || alpha < 0.0f || alpha > 1.0f)
        return 1;

    pid->USE_LOWPASS_FILTER = 1;
    LowPassFilter_Init(&pid->diff_lowpassfilter, alpha);

    return 0;
}

/**
 * @brief 设置变速积分
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量
 * @param error_integral_threshold 误差积分阈值
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetChangeIntegral(PID_Controller *pid, float error_integral_threshold)
{
    if(pid == NULL)
        return 1;

    pid->USE_CHANGE_INTEGRAL = 1;
    pid->error_integral_threshold = error_integral_threshold;

    return 0;
}

/**
 * @brief 设置积分分离
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param integral_min 积分分离下限，即积分项更新时的输入不能大于这个值。
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetIntegralSeperate(PID_Controller *pid, float integral_min, float integral_max)
{
    if(pid == NULL || integral_min > integral_max)
        return 1;

    pid->USE_INTEGRAL_SEPARATE = 1;
    pid->input_integral_min = integral_min;
    pid->input_integral_max = integral_max;

    return 0;
}

/**
 * @brief 设置积分限幅
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param integral_min 积分下限，即积分结果不能低于这个值。
 * @param integral_max 积分上限，即积分结果不能高于这个值。
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetIntegralLimit(PID_Controller *pid, float integral_min, float integral_max)
{
    if(pid == NULL || integral_min > integral_max)
        return 1;

    pid->USE_INTEGRAL_LIMIT = 1;
    pid->integral_min = integral_min;
    pid->integral_max = integral_max;

    return 0;
}

/**
 * @brief 设置输出限幅
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param output_min 输出下限，即输出结果不能低于这个值。
 * @param output_max 输出上限，即输出结果不能高于这个值。
 * @retval 0 成功; 1 失败
 */
uint8_t PID_SetOutputLimit(PID_Controller *pid, float output_min, float output_max)
{
    if(pid == NULL || output_min > output_max)
        return 1;

    pid->USE_OUTPUT_LIMIT = 1;
    pid->output_min = output_min;
    pid->output_max = output_max;

    return 0;
}

/**
 * @brief PID控制器
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param setpoint 设定值，即期望的目标值。
 * @param measuredata 测量值，即当前的实际值。
 * @retval 返回输出值
 */
float PID_Calculate(PID_Controller *pid, float error)
{
    uint32_t current_tick = HAL_GetTick();
    uint32_t dt = current_tick - pid->last_tick;
    pid->last_tick = current_tick;

    /* 更新输入 */
    pid->error = error;
    if(pid->USE_CHANGE_INTEGRAL)    // 如果使用变速积分(如果变速积分和积分分离同时设置,变速积分会执行而积分分离不会)
    {
        if(fabsf(pid->error) < pid->error_integral_threshold)
            pid->integral_error += (1 - fabsf(pid->error / pid->error_integral_threshold)) * pid->error * dt;
    }
    else if(pid->USE_INTEGRAL_SEPARATE)  // 如果使用积分分离
    {
        if( pid->error < pid->input_integral_max && pid->error > pid->input_integral_min) 
            pid->integral_error += pid->error * dt;
    }
    else
    {
        pid->integral_error += pid->error * dt;
    }
    if(pid->USE_LOWPASS_FILTER) // 如果使用低通滤波
    {
        pid->error = LowPassFilter_GetValue(&pid->diff_lowpassfilter, pid->error);
    }
    pid->rate_error = (pid->error - pid->last_error) / dt;
    if (pid->USE_INTEGRAL_LIMIT)    // 如果使用积分限幅
    {
        if (pid->integral_error > pid->integral_max)
        {
            pid->integral_error = pid->integral_max;
        }
        else if (pid->integral_error < pid->integral_min)
        {
            pid->integral_error = pid->integral_min;
        }
    }

    /* 计算P,I,D的分别输出 */
    pid->p_out = pid->Kp * pid->error;
    pid->i_out = pid->Ki * pid->integral_error;
    pid->d_out = pid->Kd * pid->rate_error;

    /* 计算总输出 */
    pid->output = pid->p_out + pid->i_out + pid->d_out;
    if (pid->USE_OUTPUT_LIMIT)  // 如果使用输出限幅
    {
        if (pid->output > pid->output_max)
        {
            pid->output = pid->output_max;
        }
        else if (pid->output < pid->output_min)
        {
            pid->output = pid->output_min;
        }
    }

    /* 保存本次的输入，用于下一次调用 */
    pid->last_error = pid->error;

    /* 返回结果 */
    return pid->output;
}

/**
 * @brief 增量式PID控制器
 * @param pid 指向 PID_Controller 结构体的指针，包含PID控制器的所有参数和状态变量。
 * @param error 误差值，即当前测量值与设定值的差值。
 * @retval 返回输出值
 */
float PID_Increment(PID_Controller *pid, float error)
{
    uint32_t current_tick = HAL_GetTick();
    uint32_t dt = current_tick - pid->last_tick;
    pid->last_tick = current_tick;

    /* 更新输入 */
    if(pid->USE_DEAD_ZONE) // 死区实现
    {
        if(fabsf(error) < pid->dead_zone)
            error = 0.0f;
        else
            pid->error = error;
    }
    else
    {
        pid->error = error;
    }
    
    /* 计算输出 */
    pid->p_out = pid->Kp * (pid->error - pid->last_error);
    if(pid->USE_CHANGE_INTEGRAL)    // 变速积分实现
        if(fabsf(pid->error) < pid->error_integral_threshold)
        {
            pid->i_out = pid->Ki * pid->error * dt * (1 - fabsf(pid->error / pid->error_integral_threshold));
        }
        else
        {
            pid->i_out = 0.0f;
        }
    else
    {
        pid->i_out = pid->Ki * pid->error * dt;
    }
    if(pid->USE_LOWPASS_FILTER) // 低通滤波实现
        pid->d_out = pid->Kd * LowPassFilter_GetValue(  &pid->diff_lowpassfilter, 
                                                        (pid->error - 2 * pid->last_error + pid->last_last_error) / dt);
    else
        pid->d_out = pid->Kd * (pid->error - 2 * pid->last_error + pid->last_last_error) / dt;
    pid->output += pid->p_out + pid->i_out + pid->d_out;
    if(pid->USE_OUTPUT_LIMIT)   // 输出限幅实现
    {
        if(pid->output > pid->output_max)
        {
            pid->output = pid->output_max;
        }
        else if(pid->output < pid->output_min)
        {
            pid->output = pid->output_min;
        }
    }

    /* 保存本次输入 */
    pid->last_last_error = pid->last_error;
    pid->last_error = pid->error;

    return pid->output;
}
