#include "task_display.h"
#include "spi.h"
#include "task_attitude.h"
#include "stdio.h"

ST7789V2_Instance *st7789v2;

uint8_t Disp_Line[20];

void StartDisplayTask(void *argument)
{
    ST7789V2_InitTypedef init =
        {
            .hspi = &hspi2,
            .res_port = GPIOC,
            .res_pin = GPIO_PIN_11,
            .dc_port = GPIOD,
            .dc_pin = GPIO_PIN_3,
            .cs_port = GPIOC,
            .cs_pin = GPIO_PIN_1,
            .blk_port = GPIOC,
            .blk_pin = GPIO_PIN_0};
    st7789v2 = ST7789V2_Register(&init);
    ST7789V2_Init(st7789v2, 0, 19);
    ST7789V2_Fill(st7789v2, 0, 0, LCD_W, LCD_H, WHITE);

    for (;;)
    {
        sprintf((char *)Disp_Line, "Pitch: %8.2f ", pitch);
        ST7789V2_ShowString(st7789v2, 10, 20, (uint8_t *)Disp_Line, RED, WHITE, 16, 0);
        sprintf((char *)Disp_Line, " Roll: %8.2f ", roll);
        ST7789V2_ShowString(st7789v2, 10, 40, (uint8_t *)Disp_Line, RED, WHITE, 16, 0);
        sprintf((char *)Disp_Line, "  Yaw: %8.2f ", yaw);
        ST7789V2_ShowString(st7789v2, 10, 60, (uint8_t *)Disp_Line, RED, WHITE, 16, 0);

        // ST7789V2_ShowString(st7789v2, 10, 20, (uint8_t *)"Pitch:", RED, WHITE, 16, 0);
        // ST7789V2_ShowFloatNum1(st7789v2, 80, 20, pitch, 6, RED, WHITE, 16);
        // ST7789V2_ShowString(st7789v2, 10, 40, (uint8_t *)"Roll:", RED, WHITE, 16, 0);
        // ST7789V2_ShowFloatNum1(st7789v2, 80, 40, roll, 6, RED, WHITE, 16);
        // ST7789V2_ShowString(st7789v2, 10, 60, (uint8_t *)"Yaw:", RED, WHITE, 16, 0);
        // ST7789V2_ShowFloatNum1(st7789v2, 80, 60, yaw, 6, RED, WHITE, 16);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
