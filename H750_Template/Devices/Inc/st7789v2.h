#ifndef __ST7789_H
#define __ST7789_H

#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stm32h750xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

/* 设置横屏或者竖屏显示 */
#define USE_HORIZONTAL (0) // 0或1为竖屏 2或3为横屏
#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 240
#define LCD_H 280
#else
#define LCD_W 280
#define LCD_H 240
#endif

/* 画笔颜色 */
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40      // 棕色
#define BRRED 0XFC07      // 棕红色
#define GRAY 0X8430       // 灰色
#define DARKBLUE 0X01CF   // 深蓝色
#define LIGHTBLUE 0X7D7C  // 浅蓝色
#define GRAYBLUE 0X5458   // 灰蓝色
#define LIGHTGREEN 0X841F // 浅绿色
#define LGRAY 0XC618      // 浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 0XA651  // 浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12     // 浅棕蓝色(选择条目的反色)

typedef struct
{
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *res_port;
    uint16_t res_pin;
    GPIO_TypeDef *dc_port;
    uint16_t dc_pin;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef *blk_port;
    uint16_t blk_pin;
} ST7789V2_InitTypedef;

typedef struct
{
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *res_port;
    uint16_t res_pin;
    GPIO_TypeDef *dc_port;
    uint16_t dc_pin;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef *blk_port;
    uint16_t blk_pin;

    /* 地址窗的偏移 */
    uint16_t xoffset;
    uint16_t yoffset;
} ST7789V2_Instance;

uint32_t mypow(uint8_t m, uint8_t n);                                                                                                                   // 求幂
void ST7789V2_Address_Set(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                                             // 设置坐标函数
ST7789V2_Instance *ST7789V2_Register(ST7789V2_InitTypedef *init);                                                                                       // 注册实例
void ST7789V2_Init(ST7789V2_Instance *instance, uint16_t xoffset, uint16_t yoffset);                                                                    // LCD初始化
void ST7789V2_Fill(ST7789V2_Instance *instance, uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);                            // 指定区域填充颜色
void ST7789V2_DrawPoint(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t color);                                                           // 在指定位置画一个点
void ST7789V2_DrawLine(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);                                // 在指定位置画一条线
void ST7789V2_DrawRectangle(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);                           // 在指定位置画一个矩形
void ST7789V2_DrawCircle(ST7789V2_Instance *instance, uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                                             // 在指定位置画一个圆
void ST7789V2_ShowChinese(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);      // 显示汉字串
void ST7789V2_ShowChinese12x12(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // 显示单个12x12汉字
void ST7789V2_ShowChinese16x16(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // 显示单个16x16汉字
void ST7789V2_ShowChinese24x24(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // 显示单个24x24汉字
void ST7789V2_ShowChinese32x32(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // 显示单个32x32汉字
void ST7789V2_ShowChar(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);        // 显示一个字符
void ST7789V2_ShowString(ST7789V2_Instance *instance, uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // 显示字符串
void ST7789V2_ShowIntNum(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);      // 显示整数变量
void ST7789V2_ShowFloatNum1(ST7789V2_Instance *instance, uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);      // 显示两位小数变量
void ST7789V2_ShowPicture(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]);                   // 显示图片

#endif
