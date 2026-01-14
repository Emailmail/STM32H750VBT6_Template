#ifndef _VOFA_H_
#define _VOFA_H_
#include "bsp_uart.h"
#define USE_FreeRTOS    (0)
#if USE_FreeRTOS
#include "FreeRTOS.h"
#endif
/*
调参帧格式规定:
0xAA | 参数序号(1Byte) | 参数值(4Byte) | 0xBB
*/

#define VOFA_MAX_PARAM 16 // 最大可调节的参数数量
#define VOFA_FRAME_HEAD 0xAA
#define VOFA_FRAME_TAIL 0xBB

typedef struct
{
    UART_Instance *uart_ins;
    float *params[VOFA_MAX_PARAM]; // 参数指针数组
    uint8_t param_cnt;             // 已经存入的参数数量(总是指向未填入的位置)
} VOFA_Instance;

typedef struct
{
    UART_HandleTypeDef *uart_handle; // 绑定的串口
} VOFA_Config_s;

VOFA_Instance *VOFA_Register(VOFA_Config_s *config);
uint8_t VOFA_Init(VOFA_Instance *instance);
uint8_t VOFA_SetParam(VOFA_Instance *instance, float *param, uint8_t num);
uint8_t VOFA_SendData(VOFA_Instance *instance, float *datas[], uint8_t len);
uint8_t VOFA_SendFrame(VOFA_Instance *instance, float data, uint8_t num);
void VOFA_DeviceCallback(void *device_instance, uint16_t size);
#endif
