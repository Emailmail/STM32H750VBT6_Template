#ifndef __X6B_H
#define __X6B_H
#include "bsp_uart.h"

typedef struct x6b_config_struct
{
    UART_HandleTypeDef *usart_handle; // 实例对应的usart_handle
} X6B_Init_Config_s;

typedef struct x6b_struct
{
    UART_Instance *uart;
    uint16_t channel[14]; // 14通道数据
} X6B_Instance;

X6B_Instance *X6B_Register(X6B_Init_Config_s *init_config);
void X6B_Analyze(void *device_instance, uint16_t size);
#endif
