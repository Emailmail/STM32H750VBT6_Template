#include "task_light.h"
#include "bsp_pwm.h"

LED_Instance *led0;
LED_Instance *led1;
uint8_t display_flag = 0;

/**
 * @brief LED任务(同时还有按键检测)
 */
void StartLightTask(void *argument)
{
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

    for (;;)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET) // KEY0按下
        {
            if(display_flag == 0)
            {
                display_flag = 1;
                LED_On(led0);
            }
            else
            {
                display_flag = 0;
                LED_Off(led0);
            }
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) // KEY1按下
        {
            
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
