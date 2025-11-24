#include "bsp_uart.h"

/* usart服务实例,所有注册了usart的模块信息会被保存在这里 */
static uint8_t idx;
static UART_Instance *usart_instance[DEVICE_UART_CNT] = {NULL};

/**
 * @brief 串口实例注册
 * @param init_config 串口初始化配置
 * @retval 串口实例句柄
 */
UART_Instance *UART_Register(UART_Init_Config_s *init_config)
{
    /* 检查是否超过最大实例数量 */
    if (idx >= DEVICE_UART_CNT)
        return NULL;

    /* 检查是否已经注册过此串口 */
    for (uint8_t i = 0; i < idx; i++)
        if (usart_instance[i]->usart_handle == init_config->usart_handle)
            return NULL;

    /* 分配内存,传递指针 */
    UART_Instance *instance = (UART_Instance *)malloc(sizeof(UART_Instance));
    if(instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(UART_Instance));

    /* 参数配置 */
    instance->usart_handle = init_config->usart_handle;
    instance->recv_buff_size = init_config->recv_buff_size;
    instance->module_callback = init_config->module_callback;

    /* 在静态数组内注册该串口,且idx自增 */
    usart_instance[idx++] = instance;

    /* 开启接收中断 */
    UART_Service_Init(instance);

    /* 返回句柄 */
    return instance;
}

/**
 * @brief 启动串口服务,会在每个实例注册之后自动启用接收,当前实现为DMA接收
 * @param uart_instance 串口实例
 * @retval 启动成功返回HAL_OK,失败返回HAL_ERROR
 */
HAL_StatusTypeDef UART_Service_Init(UART_Instance *uart_instance)
{
    HAL_StatusTypeDef ret;
    ret = HAL_UARTEx_ReceiveToIdle_DMA(uart_instance->usart_handle, uart_instance->recv_buff, uart_instance->recv_buff_size);
    __HAL_DMA_DISABLE_IT(uart_instance->usart_handle->hdmarx, DMA_IT_HT);
    return ret;
}

/**
 * @brief 串口发送
 * @param uart_instance 串口句柄
 * @param send_buf 发送缓冲区
 * @param send_size 发送长度
 * @param mode 发送模式
 * @retval 发送结果,具体意义参考HAL_StatusTypeDef的定义
 */
HAL_StatusTypeDef UART_Send(UART_Instance *uart_instance, uint8_t *send_buf, uint16_t send_size, UART_TRANSFER_MODE mode)
{
    switch (mode)
    {
    case UART_TRANSFER_BLOCKING:
        return HAL_UART_Transmit(uart_instance->usart_handle, send_buf, send_size, 100);
        break;
    case UART_TRANSFER_IT:
        return HAL_UART_Transmit_IT(uart_instance->usart_handle, send_buf, send_size);
        break;
    case UART_TRANSFER_DMA:
        return HAL_UART_Transmit_DMA(uart_instance->usart_handle, send_buf, send_size);
        break;
    default:
        return HAL_ERROR;
        break;
    }
}

/**
 * @brief 查看该串口是否空闲(防止因为短时间内重复调用发送导致丢包)
 * @param uart_instance 串口实例
 * @retval 1 串口空闲 
 *         0 串口忙
 */
uint8_t UART_IsReady(UART_Instance *uart_instance)
{
    if (uart_instance->usart_handle->gState | HAL_UART_STATE_BUSY_TX)
        return 0;
    else
        return 1;
}

/**
 * @brief 每次dma/idle中断发生时，都会调用此函数.对于每个uart实例会调用对应的回调进行进一步的处理
 * @param huart 发生中断的串口
 * @param Size 此次接收到的总数据量
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* 查找是哪个串口触发的中断 */
    for (uint8_t i = 0; i < idx; ++i)
    {
        if (huart == usart_instance[i]->usart_handle)
        {
            /* 如果定义了回调函数,就调用 */
            if (usart_instance[i]->module_callback != NULL)
            {
                usart_instance[i]->module_callback(usart_instance[i]->device_instance, Size);
                memset(usart_instance[i]->recv_buff, 0, Size); // 接收结束后清空buffer,对于变长数据是必要的
            }

            /* 再次启用接收 */
            HAL_UARTEx_ReceiveToIdle_DMA(usart_instance[i]->usart_handle, usart_instance[i]->recv_buff, usart_instance[i]->recv_buff_size);
            __HAL_DMA_DISABLE_IT(usart_instance[i]->usart_handle->hdmarx, DMA_IT_HT);
            return;
        }
    }
}

/**
 * @brief 当串口发送/接收出现错误时,会调用此函数,此时这个函数要做的就是重新启动接收
 * @note  最常见的错误:奇偶校验/溢出/帧错误
 * @param huart 发生错误的串口
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    for (uint8_t i = 0; i < idx; ++i)
    {
        if (huart == usart_instance[i]->usart_handle)
        {
            /* 再次启用接收 */
            HAL_UARTEx_ReceiveToIdle_DMA(usart_instance[i]->usart_handle, usart_instance[i]->recv_buff, usart_instance[i]->recv_buff_size);
            __HAL_DMA_DISABLE_IT(usart_instance[i]->usart_handle->hdmarx, DMA_IT_HT);
            return;
        }
    }
}