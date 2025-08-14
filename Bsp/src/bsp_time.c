#include "bsp.h"


void time_tim1_pwm_config(uint16_t pulse_width)
{
	LL_TIM_DisableCounter(TIM1);
	LL_TIM_SetAutoReload(TIM1,39);
	LL_TIM_OC_SetCompareCH2(TIM1,pulse_width);//LL_TIM_OC_SetCompareCH1(TIM1,pulse_width);
	LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM1);
	LL_TIM_EnableAllOutputs(TIM1); //advance timer is need statemt .
}

/************************************************************
************************************************************/
void time_tim1_pwm_stop(void)
{
    LL_TIM_DisableAllOutputs(TIM1);
    LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH2);
    LL_TIM_DisableCounter(TIM1);
}


#if 0

// =======================
// PWM 启动（统一版本）
// =======================
static inline void pwm_start(TIM_TypeDef *TIMx, uint32_t channel, uint16_t pulse)
{
    LL_TIM_SetAutoReload(TIMx, 39);
    LL_TIM_OC_SetCompareCH1(TIMx, pulse);  // 如果 channel 不是 CH1，可做参数化
    LL_TIM_CC_EnableChannel(TIMx, channel);
    LL_TIM_EnableCounter(TIMx);

    // 高级定时器才有 MOE
    if ((TIMx == TIM1) || (TIMx == TIM8))
    {
        LL_TIM_EnableAllOutputs(TIMx); // MOE=1
    }
}

// =======================
// PWM 停止（统一版本）
// =======================
static inline void pwm_stop(TIM_TypeDef *TIMx, uint32_t channel)
{
    // 高级定时器才有 MOE
    if ((TIMx == TIM1) || (TIMx == TIM8))
    {
        LL_TIM_DisableAllOutputs(TIMx); // MOE=0-main output enable
    }

    LL_TIM_CC_DisableChannel(TIMx, channel);
    LL_TIM_DisableCounter(TIMx);
}




#endif 
