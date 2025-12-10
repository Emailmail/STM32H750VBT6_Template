#ifndef __OLED_H__
#define __OLED_H__

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_i2c.h"
#include <math.h>
#include <stdlib.h>
#include "font.h"
#include "i2c.h"
#include "string.h"

#define OLED_ADDRESS 0x78

#define OLED_PAGE 8            // OLED页数
#define OLED_ROW 8 * OLED_PAGE // OLED行数
#define OLED_COLUMN 128        // OLED列数

typedef enum
{
  OLED_COLOR_NORMAL = 0, // 正常模式 黑底白字
  OLED_COLOR_REVERSED    // 反色模式 白底黑字
} OLED_ColorMode;

typedef struct 
{
  I2C_HandleTypeDef *hi2c;
} OLED_InitTypedef;

typedef struct
{
  I2C_HandleTypeDef *hi2c;
  uint8_t GRAM[OLED_PAGE][OLED_COLUMN];
} OLED_Instance;

OLED_Instance *OLED_Register(OLED_InitTypedef *init);
void OLED_Init(OLED_Instance *instance);
void OLED_DisPlay_On(OLED_Instance *instance);
void OLED_DisPlay_Off(OLED_Instance *instance);

void OLED_NewFrame(OLED_Instance *instance);
void OLED_ShowFrame(OLED_Instance *instance);
void OLED_SetPixel(OLED_Instance *instance, uint8_t x, uint8_t y, OLED_ColorMode color);

void OLED_DrawLine(OLED_Instance *instance, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_ColorMode color);
void OLED_DrawRectangle(OLED_Instance *instance, uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawFilledRectangle(OLED_Instance *instance, uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawTriangle(OLED_Instance *instance, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color);
void OLED_DrawFilledTriangle(OLED_Instance *instance, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color);
void OLED_DrawCircle(OLED_Instance *instance, uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawFilledCircle(OLED_Instance *instance, uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawEllipse(OLED_Instance *instance, uint8_t x, uint8_t y, uint8_t a, uint8_t b, OLED_ColorMode color);
void OLED_DrawImage(OLED_Instance *instance, uint8_t x, uint8_t y, const Image *img, OLED_ColorMode color);

void OLED_PrintASCIIChar(OLED_Instance *instance, uint8_t x, uint8_t y, char ch, const ASCIIFont *font, OLED_ColorMode color);
void OLED_PrintASCIIString(OLED_Instance *instance, uint8_t x, uint8_t y, char *str, const ASCIIFont *font, OLED_ColorMode color);
void OLED_PrintString(OLED_Instance *instance, uint8_t x, uint8_t y, char *str, const Font *font, OLED_ColorMode color);

#endif