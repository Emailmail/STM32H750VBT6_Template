#include "key.h"

/**
 * @brief 按键注册
 */
Key_Instance *Key_Register(Key_InitTypedef *init)
{
    if (init == NULL)
        return NULL;

    Key_Instance *instance = malloc(sizeof(Key_Instance));
    if (instance == NULL)
        return NULL;

    instance->pin = init->pin;
    instance->port = init->port;
    instance->attr = init->attr;
    instance->delay_count = DELAY_TIME / init->dt;
    if (instance->delay_count < 1)
        instance->delay_count = 1;

    instance->count = 0;
    instance->state = 0;

    return instance;
}

/**
 * @brief 按键检测(需要按照一定周期调用此函数)
 */
bool Key_Detect(Key_Instance *instance)
{
    /* 检测电平 */
    bool level = (bool)HAL_GPIO_ReadPin(instance->port, instance->pin);

    switch (instance->count)
    {
    /* 未被按下,若检测到电平改变(即认为被按下)则进入下一阶段 */
    case 0:
        if (level == !instance->attr)
        {
            instance->count = 1;
            instance->timer = 0;
        }
        break;

    /* 消抖 */
    case 1:
        if (instance->timer < instance->delay_count)
        {
            instance->timer++;
        }
        else
        {
            /* 消抖完成，确认按键状态 */
            if (level == !instance->attr)
            {
                instance->count = 2;
                instance->timer = 0;
                instance->state = 1;  // 确认按键按下
            }
            else
            {
                instance->count = 0;    // 消抖期间恢复原状，视为抖动
            }
        }
        break;

    /* 检测到电平恢复,则认为已经松开,进入下一阶段 */
    case 2:
        if (level == instance->attr)
        {
            instance->count = 3;
            instance->timer = 0;
        }
        break;

    /* 再次消抖,消抖完毕后重新开始新一轮检测 */
    case 3:
        if(instance->timer < instance->delay_count)
        {
            instance->timer++;
        }
        else
        {
            /* 消抖完成，确认释放状态 */
            if (level == instance->attr)
            {
                instance->count = 0;
                instance->state = 0;  // 确认按键释放
            }
            else
            {
                instance->count = 2;    // 消抖期间恢复原状，视为抖动
            }
        }
        break;
    }

    /* 返回按键状态 */
    return instance->state;
}
