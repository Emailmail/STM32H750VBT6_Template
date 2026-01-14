#include "vofa.h"

const uint8_t vofa_justfloat_tail[4] = {0x00, 0x00, 0x80, 0x7F};

/**
 * @brief VOFA将一个浮点数化为4字节数组后依次发送出去
 * @param instance VOFA实例
 * @param data 浮点数
 */
static void VOFA_SendFloat(VOFA_Instance *instance, float f)
{
    union 
    {
        float value;
        uint8_t array[4];
    } data;
    data.value = f;
    UART_Send(instance->uart_ins, data.array, 4, UART_TRANSFER_DMA);
}

/**
 * @brief 注册VOFA模块
 * @param config 配置参数
 */
VOFA_Instance *VOFA_Register(VOFA_Config_s *config)
{
    /* 参数检验 */
    if (config == NULL)
        return NULL;
    else if (config->uart_handle == NULL)
        return NULL;

    /* 注册VOFA所需的串口实例 */
    UART_Init_Config_s vofa_uart_config =
        {
            .usart_handle = config->uart_handle,
            .module_callback = VOFA_DeviceCallback
        };
    UART_Instance *vofa_uart_instance = UART_Register(&vofa_uart_config);
    if (vofa_uart_instance == NULL)
        return NULL;

    /* 注册VOFA实例 */
#if USE_FreeRTOS
    VOFA_Instance *instance = (VOFA_Instance *)pvPortMalloc(sizeof(VOFA_Instance));
#else
    VOFA_Instance *instance = (VOFA_Instance *)malloc(sizeof(VOFA_Instance));
#endif
    if (instance == NULL)
        return NULL;
    memset(instance, 0, sizeof(VOFA_Instance));

    /* 参数传递 */
    vofa_uart_instance->device_instance = (void *)instance; // 将VOFA实例反过来挂载到UART实例上(注:此步骤影响回调函数实现)
    instance->uart_ins = vofa_uart_instance;

    return instance;
}

/**
 * @brief VOFA模块初始化
 * @param instance VOFA实例
 */
uint8_t VOFA_Init(VOFA_Instance *instance)
{
    if(UART_Service_Init(instance->uart_ins) != HAL_OK)
        return 1;
    else
        return 0;
}

/**
 * @brief VOFA模块填入被调参数
 * @param param 参数地址
 * @param num 参数序号
 * @retval 0:成功; 1:失败
 */
uint8_t VOFA_SetParam(VOFA_Instance *instance, float *param, uint8_t num)
{
    /* 检验是否已满 */
    if(instance->param_cnt >= VOFA_MAX_PARAM)
        return 1;

    /* 检验param是否为NULL*/
    if(param == NULL)
        return 1;

    /* 检验num是否在有效范围内 */
    if(num >= VOFA_MAX_PARAM)
        return 1;

    /* 检验指定的序号是否已填入参数 */
    if(instance->params[num] != NULL)
        return 1;

    /* 填入参数 */
    instance->params[num] = param;
    instance->param_cnt++;
    return 0;
}

/**
 * @brief VOFA发送数据(justfloat格式)
 * @param instance VOFA实例
 * @param datas 要发送的数据的地址的数组
 * @param len 要发送的数据长度
 * @retval 0:成功; 1:失败
 * @note 一般用于发送给电脑
 */
uint8_t VOFA_SendData(VOFA_Instance *instance, float *datas[], uint8_t len)
{
    /* 分配内存空间 */
    uint8_t i;
    uint8_t buffer_size = len * 4 + 4;  // 字节数 = (数据长度 + 帧尾) * 4

#if USE_FreeRTOS
    uint8_t *buffer = (uint8_t *)pvPortMalloc(buffer_size);
#else
    uint8_t *buffer = malloc(buffer_size);
#endif
    if (buffer == NULL)
        return 1;

    /* 将浮点数合并到一个字节数组中 */
    uint8_t *dest = buffer;
    for (i = 0; i < len; i++) 
    {
        if (datas[i] == NULL) 
        {
            free(buffer);
            return 1;
        }
        memcpy(dest, datas[i], 4);
        dest += 4;
    }

    /* 合并帧尾 */
    memcpy(dest, vofa_justfloat_tail, 4);

    /* 一次性发送完整帧 */
    if(UART_Send(instance->uart_ins, buffer, buffer_size, UART_TRANSFER_IT) != HAL_OK)
        return 1;

    /* 释放内存空间 */
#if USE_FreeRTOS
    vPortFree(buffer);
#else
    free(buffer);
#endif

    return 0;
}

/**
 * @brief 按照VOFA通信协议的格式发送一帧
 * @param instance VOFA实例
 * @param data 要发送的数据
 * @param num 参数序号
 * @note 一般用于发送给其它MCU
 */
uint8_t VOFA_SendFrame(VOFA_Instance *instance, float data, uint8_t num)
{
    uint8_t buffer[7];
    buffer[0] = VOFA_FRAME_HEAD;
    buffer[1] = num;
    
    union {
        float f;
        uint8_t bytes[4];
    } data_union;
    data_union.f = data;
    memcpy(&buffer[2], data_union.bytes, 4);
    
    buffer[6] = VOFA_FRAME_TAIL;

    if (UART_Send(instance->uart_ins, buffer, 7, UART_TRANSFER_DMA) != HAL_OK)
        return 1;
    
    return 0;
}

/**
 * @brief VOFA模块回调函数
 */
void VOFA_DeviceCallback(void *device_instance, uint16_t size)
{
    uint8_t i;
    VOFA_Instance *instance = (VOFA_Instance *)device_instance;
    union
    {
        uint8_t array[4];
        float value;
    } vofa_data;

    /* 长度检验 */
    if (size != 7)
        return;

    /* 格式检验 */
    if (instance->uart_ins->recv_buff[0] != VOFA_FRAME_HEAD || instance->uart_ins->recv_buff[6] != VOFA_FRAME_TAIL)
        return;

    /* 检验指定的被调参数的序号是否在有效范围内 */
    if(instance->uart_ins->recv_buff[1] >= VOFA_MAX_PARAM)
        return;

    /* 检验指定的被调参数是否存在 */
    if (instance->params[instance->uart_ins->recv_buff[1]] == NULL)
        return;

    /* 4字节转浮点数 */
    for (i = 0; i < 4; i++)
    {
        vofa_data.array[i] = instance->uart_ins->recv_buff[i + 2];
    }

    /* 调节指定的参数 */
    *instance->params[instance->uart_ins->recv_buff[1]] = vofa_data.value;
}
