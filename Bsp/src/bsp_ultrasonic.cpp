#include "bsp.h"


void ultra_class::ultra_output_fun(void)
{
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);//ultrasnoic On

}

void ultra_class::ultra_stop_fun(void)
{

    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);//ultrasnoic Off



}

