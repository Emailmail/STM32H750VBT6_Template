#include "task_debug.h"
#include "task_attitude.h"
#include "task_motor.h"
#include "usart.h"

VOFA_Instance *vofa_debug;
float *send_param[2];

void StartDebugTask(void *argument)
{
    static TickType_t xLastWakeTime = 0;
    const TickType_t xPeriod = pdMS_TO_TICKS(50);

    VOFA_Config_s vofa_debug_config = {
        .uart_handle = &huart1,
    };
    vofa_debug = VOFA_Register(&vofa_debug_config);
    send_param[0] = &tb6612_motor1->speed;
    send_param[1] = &tb6612_motor2->speed;

    xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        VOFA_SendData(vofa_debug, send_param, 2);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
