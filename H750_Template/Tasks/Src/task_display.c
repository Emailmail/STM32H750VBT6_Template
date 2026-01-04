#include "task_display.h"
#include "spi.h"

ST7789V2_Instance *st7789v2;

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
    ST7789V2_Init(st7789v2);
    ST7789V2_Fill(st7789v2, 0, 0, LCD_W, LCD_H, WHITE);

    for (;;)
    {
        // ST7789V2_ShowChinese(st7789v2, 0, 180, (uint8_t *)"中景园电子", RED, WHITE, 32, 0);
        ST7789V2_ShowString(st7789v2, 0, 0, (uint8_t *)"LCD_W:", RED, WHITE, 16, 0);
        ST7789V2_ShowIntNum(st7789v2, 48, 0, LCD_W, 3, RED, WHITE, 16);
        ST7789V2_ShowString(st7789v2, 80, 0, (uint8_t *)"LCD_H:", RED, WHITE, 16, 0);
        ST7789V2_ShowIntNum(st7789v2, 128, 0, LCD_H, 3, RED, WHITE, 16);
        ST7789V2_ShowString(st7789v2, 80, 0, (uint8_t *)"LCD_H:", RED, WHITE, 16, 0);
        ST7789V2_ShowString(st7789v2, 0, 30, (uint8_t *)"Increaseing Nun:", RED, WHITE, 16, 0);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
