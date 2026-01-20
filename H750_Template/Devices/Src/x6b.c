#include "x6b.h"

X6B_Instance *X6B_Register(X6B_Init_Config_s *init_config)
{
    /* 注册一个串口实例 */
    UART_Init_Config_s uart_init_config = 
    {
        .usart_handle = init_config->usart_handle,
        .recv_buff_size = 64, 
        .module_callback = X6B_Analyze,
    };
    UART_Instance *uart_instance = UART_Register(&uart_init_config);
    if(uart_instance == NULL)
        return NULL;

    /* 分配一个X6B实例的内存 */
#if USE_FREERTOS
    X6B_Instance *instance = (X6B_Instance *)pvPortMalloc(sizeof(X6B_Instance));
#else
    X6B_Instance *instance = (X6B_Instance *)malloc(sizeof(X6B_Instance));
#endif
    if(instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(X6B_Instance));

    /* 传递参数 */
    instance->uart = uart_instance;
    instance->uart->device_instance = instance;

    return instance;
}

void X6B_Analyze(void *device_instance, uint16_t size)
{
    X6B_Instance *instance = (X6B_Instance *)device_instance;

    /* 数据长度校验 */
    if(size != 32)
        return;

    /* 帧头校验 */
    if(instance->uart->recv_buff[0] != 0x20 || instance->uart->recv_buff[1] != 0x40)
        return;

    /* 通道数据解析 */
    for(uint8_t i = 0; i < 14; ++i)
    {
        instance->channel[i] = (instance->uart->recv_buff[i * 2 + 3] << 8) | instance->uart->recv_buff[i * 2 + 2];
    }
    return;
}
