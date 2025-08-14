#include "bsp.h"


void ultra_output_fun(void)
{
    
    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);//ultrasnoic On
    time_tim1_pwm_config(20);
    
}

void ultra_stop_fun(void)
{

    //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);//ultrasnoic Off
    time_tim1_pwm_stop();
	


}

