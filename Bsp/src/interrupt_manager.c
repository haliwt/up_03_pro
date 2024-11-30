#include "bsp.h"

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function call back function
	*timer 1ms 
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  static uint16_t tm0;
  static uint8_t tm1;
  

  if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	if(tm0 > 999){
      tm0=0;
      tm1++;

      gpro_t.gTimer_adc_detected_time++;
   
      gpro_t.gTimer_normal_run_main_function_times++;
      gpro_t.gTimer_timer_time_long_key ++;

      if(tm1 > 59){ //1 minute.
         tm1 =0;
         gpro_t.gTimer_set_minutes ++ ;
         gpro_t.gTimer_works_total_times_minutes++ ;
        

      }

    }
    
	}
}






