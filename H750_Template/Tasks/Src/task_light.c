#include "task_light.h"
#include "bsp_pwm.h"

LED_Instance *led0;
LED_Instance *led1;

PWM_Instance *pwm1;
PWM_Instance *pwm2;

void StartLightTask(void *argument)
{
    static TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = 100;
    xLastWakeTime = xTaskGetTickCount();

    LED_InitTypedef led0_init =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_5, // LED0在PE5上
            .attr = 1          // LED0为高电平时点亮
        };
    LED_InitTypedef led1_init =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_6, // LED1在PE6上
            .attr = 1          // LED1为高电平时点亮
        };
    led0 = LED_Register(&led0_init);
    led1 = LED_Register(&led1_init);

    PWM_Init_Config_s pwm1_config = {
        .channel = TIM_CHANNEL_1,
        .dutyratio = 0.5,
        .htim = &htim15,
        .period = 0.001,
    };
    pwm1 = PWM_Register(&pwm1_config);
        PWM_Init_Config_s pwm2_config = {
        .channel = TIM_CHANNEL_2,
        .dutyratio = 0.5,
        .htim = &htim15,
        .period = 0.001,
    };
    pwm2 = PWM_Register(&pwm2_config);

    uint8_t flag = 0;
    for (;;)
    {
        if (flag)
        {
            // LED_On(led0);
            // LED_Off(led1);
            // flag = 0;
            PWM_SetDutyRatio(pwm1, pwm1->dutyratio + 0.1);
            PWM_SetDutyRatio(pwm2, pwm2->dutyratio - 0.1);
            if(pwm1->dutyratio > 0.9)
            {
                flag = 0;
            }
        }
        else
        {
            // LED_Off(led0);
            // LED_On(led1);
            // flag = 1;
            PWM_SetDutyRatio(pwm1, pwm1->dutyratio - 0.1);
            PWM_SetDutyRatio(pwm2, pwm2->dutyratio + 0.1);
            if(pwm1->dutyratio < 0.1)
            {
                flag = 1;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
