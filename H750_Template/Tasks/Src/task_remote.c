#include "task_remote.h"

float right_x = 0.0f;
float right_y = 0.0f;
float left_x = 0.0f;
float left_y = 0.0f;

X6B_Instance *x6b;

/**
 * @brief X6B接收机解析任务
 */
void StartRemoteTask(void *argument)
{
    const TickType_t xPeriod = pdMS_TO_TICKS(50);
    TickType_t xLastWakeTime = 0;

    /* 注册X6B接收机 */
    X6B_Init_Config_s x6b_init_config =
        {
            .usart_handle = &huart2,
        };
    x6b = X6B_Register(&x6b_init_config);
    if (x6b == NULL)
    {
        while (1)
            ;
    }

    xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        /* 解析左右摇杆的X、Y轴数据 */
        right_x = (float)(x6b->channel[0] - 1500.0f) / 500.0f;
        right_y = (float)(x6b->channel[1] - 1500.0f) / 500.0f;
        left_y = (float)(x6b->channel[2] - 1500.0f) / 500.0f;
        left_x = (float)(x6b->channel[3] - 1500.0f) / 500.0f;
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
