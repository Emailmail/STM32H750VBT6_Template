#include "task_motor.h"

TB6612_Instance *tb6612_motor1;
TB6612_Instance *tb6612_motor2;

float duty = 0.0f;

void StartMotorTask(void *argument)
{
    static TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(50);

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

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); // 使能TB6612
    TB6612_MotorStart(tb6612_motor1);
    TB6612_MotorStart(tb6612_motor2);
    TB6612_GetSpeed_Start(tb6612_motor1);
    TB6612_GetSpeed_Start(tb6612_motor2);
    
    xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        TB6612_SetDuty(tb6612_motor1, duty);
        TB6612_SetDuty(tb6612_motor2, duty);

        TB6612_GetSpeed(tb6612_motor1);
        TB6612_GetSpeed(tb6612_motor2);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
