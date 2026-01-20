#include "task_motor.h"

TB6612_Instance *tb6612_motor1;
TB6612_Instance *tb6612_motor2;

/* PWM占空比输出 */
float duty = 0.0f;
float delta_dutyleft = 0.0f;
float delta_dutyright = 0.0f;

/* 小车速度和差速 */
float speed = 0.0f;
float deltaspeed = 0.0f;

/**
 * @brief 电机控制任务
 */
void StartMotorTask(void *argument)
{
    static TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(5);

    static uint32_t count = 0;
    static uint32_t safe_count = 0; // 时间计数，防止电机连续满转

    /* 注册TB6612的Motor对象 */
    PWM_Init_Config_s pwm_motor1_init =
        {
            .htim = &htim12,
            .channel = TIM_CHANNEL_2,
            .period = 0.0001,
        };
    TB6612_InitTypedef tb6612_motor1_init =
        {
            .hencoder = &htim2,
            .channelA = TIM_CHANNEL_1,
            .channelB = TIM_CHANNEL_2,
            .in1_port = GPIOC,
            .in1_pin = GPIO_PIN_6,
            .in2_port = GPIOC,
            .in2_pin = GPIO_PIN_7,
            .ppr = 13,
            .reduction_ratio = 20,
            .pwm_init = &pwm_motor1_init,
        };
    tb6612_motor1 = TB6612_Register(&tb6612_motor1_init);
    PWM_Init_Config_s pwm_motor2_init =
        {
            .htim = &htim12,
            .channel = TIM_CHANNEL_1,
            .period = 0.0001,
        };
    TB6612_InitTypedef tb6612_motor2_init =
        {
            .hencoder = &htim3,
            .channelA = TIM_CHANNEL_1,
            .channelB = TIM_CHANNEL_2,
            .in1_port = GPIOB,
            .in1_pin = GPIO_PIN_12,
            .in2_port = GPIOB,
            .in2_pin = GPIO_PIN_13,
            .ppr = 13,
            .reduction_ratio = 20,
            .pwm_init = &pwm_motor2_init,
        };
    tb6612_motor2 = TB6612_Register(&tb6612_motor2_init);

    /* 使能模块 s*/
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); // 使能TB6612
    TB6612_MotorStart(tb6612_motor1);   // 启动电机1PWM输出
    TB6612_MotorStart(tb6612_motor2);   // 启动电机2PWM输出
    TB6612_GetSpeed_Start(tb6612_motor1);   // 启动电机1编码器
    TB6612_GetSpeed_Start(tb6612_motor2);   // 启动电机2编码器

    xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        /* 设置PWM占空比 */
        TB6612_SetDuty(tb6612_motor1, duty + delta_dutyleft);   // 设置电机1占空比
        TB6612_SetDuty(tb6612_motor2, duty + delta_dutyright);  // 设置电机2占空比

        /* 安全控制，防止电机连续满转 */
        count += xPeriod;
        if (duty > 0.9f || duty < -0.9f)
        {
            safe_count += xPeriod;
            if (safe_count >= pdMS_TO_TICKS(200))
            {
                TB6612_SetDuty(tb6612_motor1, 0.0f);
                TB6612_SetDuty(tb6612_motor2, 0.0f);
                while (1)
                    ;
            }
        }
        else
        {
            safe_count = 0;
        }

        /* 获取电机速度 */
        if (count >= pdMS_TO_TICKS(50))
        {
            count = 0;
            TB6612_GetSpeed(tb6612_motor1);
            TB6612_GetSpeed(tb6612_motor2);
            speed = (tb6612_motor1->speed + tb6612_motor2->speed) / 2.0f;
            deltaspeed = tb6612_motor1->speed - tb6612_motor2->speed;
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
