#include "bsp_pwm.h"

static uint8_t idx;
static PWM_Instance *pwm_instance[PWM_DEVICE_CNT] = {NULL}; // 所有的pwm instance保存于此,用于callback时判断中断来源
static uint32_t PWM_SelectTclk(TIM_HandleTypeDef *htim );

#if (BSP_PWM_CALLBACK)
/**
 * @brief pwm dma传输完成回调函数
 *
 * @param htim 发生中断的定时器句柄
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    for (uint8_t i = 0; i < idx; i++)
    { // 来自同一个定时器的中断且通道相同
        if (pwm_instance[i]->htim == htim && htim->Channel == (1<<(pwm_instance[i]->channel/4)))
        {
#if (BSP_PWM_CALLBACK)
            if (pwm_instance[i]->callback) // 如果有回调函数
                pwm_instance[i]->callback(pwm_instance[i]);
#endif
            return; // 一次只能有一个通道的中断,所以直接返回
        }
    }
}
#endif

PWM_Instance *PWM_Register(PWM_Init_Config_s *config)
{
    if (idx >= PWM_DEVICE_CNT) // 超过最大实例数,考虑增加或查看是否有内存泄漏
        while (1)
            ;
    PWM_Instance *pwm = (PWM_Instance *)malloc(sizeof(PWM_Instance));
    memset(pwm, 0, sizeof(PWM_Instance));

    pwm->htim = config->htim;
    pwm->channel = config->channel;
    pwm->period = config->period;
    pwm->dutyratio = config->dutyratio;
#if (BSP_PWM_CALLBACK)
    pwm->callback = config->callback;
    pwm->id = config->id;
#endif
    pwm->tclk = PWM_SelectTclk(pwm->htim);

    HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
    PWM_SetPeriod(pwm, pwm->period);
    PWM_SetDutyRatio(pwm, pwm->dutyratio);

    pwm_instance[idx++] = pwm;

    return pwm;
}


void PWM_Start(PWM_Instance *pwm)
{
    HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
}

void PWM_Stop(PWM_Instance *pwm)
{
    HAL_TIM_PWM_Stop(pwm->htim, pwm->channel);
}

/**
 * @brief 设置pwm周期
 * @param pwm pwm实例
 * @param period 周期 单位 s
 */
void PWM_SetPeriod(PWM_Instance *pwm, float period)
{
    pwm->period = period;
    __HAL_TIM_SetAutoreload(pwm->htim, period*((pwm->tclk)/(pwm->htim->Init.Prescaler+1)));
}
/**
 * @brief 设置pwm占空比
 * @param pwm pwm实例
 * @param dutyratio 占空比 0~1
*/
void PWM_SetDutyRatio(PWM_Instance *pwm, float dutyratio)
{
    pwm->dutyratio = dutyratio;
    __HAL_TIM_SetCompare(pwm->htim, pwm->channel, dutyratio * (pwm->htim->Instance->ARR));
}

void PWM_StartDMA(PWM_Instance *pwm, uint32_t *pData, uint32_t Size)
{
    HAL_TIM_PWM_Start_DMA(pwm->htim, pwm->channel, pData, Size);
}

static uint32_t PWM_SelectTclk(TIM_HandleTypeDef *htim)
{
    uint32_t pclk_freq;
    uint32_t timer_addr = (uint32_t)(htim->Instance);   // TIM地址,根据地址判断APB总线
    
    /* APB1总线上的定时器 (TIM2, TIM3, TIM4, TIM5, TIM12, TIM13, TIM14) */
    if (timer_addr == TIM2_BASE || timer_addr == TIM3_BASE || 
        timer_addr == TIM4_BASE || timer_addr == TIM5_BASE ||
        timer_addr == TIM12_BASE || timer_addr == TIM13_BASE || timer_addr == TIM14_BASE)
    {
        pclk_freq = HAL_RCC_GetPCLK1Freq();
        uint32_t apb1_prescaler = (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) >> RCC_D2CFGR_D2PPRE1_Pos;
        if(apb1_prescaler > 0) { // APB1 prescaler > 1
            return pclk_freq * 2;
        } else {
            return pclk_freq;
        }
    }
    /* APB2总线上的定时器 (TIM1, TIM8, TIM15, TIM16, TIM17) */
    else if (timer_addr == TIM1_BASE || timer_addr == TIM8_BASE || 
             timer_addr == TIM15_BASE || timer_addr == TIM16_BASE || timer_addr == TIM17_BASE)
    {
        pclk_freq = HAL_RCC_GetPCLK2Freq();
        uint32_t apb2_prescaler = (RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) >> RCC_D2CFGR_D2PPRE2_Pos;
        if(apb2_prescaler > 0) { // APB2 prescaler > 1
            return pclk_freq * 2;
        } else {
            return pclk_freq;
        }
    }
    
    return 0;
}
