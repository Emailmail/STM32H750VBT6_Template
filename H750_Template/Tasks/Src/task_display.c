#include "task_display.h"
#include "spi.h"
#include "task_attitude.h"
#include "task_control.h"
#include "stdio.h"
#include "task_light.h"

ST7789V2_Instance *st7789v2;
extern int distance;

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
    ST7789V2_Fill(st7789v2, 0, 0, LCD_W, LCD_H, BLACK);
    
    vTaskDelay(pdMS_TO_TICKS(100));
    for (;;)
    {
        if(display_flag)
        {
            sprintf((char *)Disp_Line, "Pitch: %7.1f ", -pitch);
            ST7789V2_ShowString(st7789v2, 10, 20, (uint8_t *)Disp_Line, RED, WHITE, 32, 0);
            sprintf((char *)Disp_Line, " Roll: %7.1f ", -roll);
            ST7789V2_ShowString(st7789v2, 10, 60, (uint8_t *)Disp_Line, BROWN, WHITE, 32, 0);
            sprintf((char *)Disp_Line, "Speed: %7.1f ", speed);
            ST7789V2_ShowString(st7789v2, 10, 100, (uint8_t *)Disp_Line, BLUE, WHITE, 32, 0);
            sprintf((char *)Disp_Line, "  Dis: %7d ", distance);
            ST7789V2_ShowString(st7789v2, 10, 140, (uint8_t *)Disp_Line, GREEN, WHITE, 32, 0);
        }
        else
        {
            ST7789V2_Fill(st7789v2, 0, 0, LCD_W, LCD_H, BLACK);
        }
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
