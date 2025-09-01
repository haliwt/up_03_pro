#include "bsp.h"





void fan_output_fun(void)
{
  FAN_CTL_SetHigh();
}
void fan_stop_fun(void)
{

  FAN_CTL_SetLow();

}

/**
 * 
 * @brief  风扇故障处理函数
 * @note   该函数用于处理风扇故障情况，例如停止风扇或发出警告。
 * @param  None
 * @return None
 * @details 该函数可以在风扇检测到异常时调用，例如风
 * 扇转速异常或风扇无法启动时。可以在函数内部添加具体的处理逻辑，如停止风扇、发出警告等。
 */

void fan_fault_handler(void)
{
  if(gpro_t.gTimer_led_blink_counter > 0){
    gpro_t.gTimer_led_blink_counter=0;
    GPIO_LED_TOGGLE() ;
  
  }
}





