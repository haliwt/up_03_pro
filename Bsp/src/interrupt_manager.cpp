#include "bsp.h"

uint8_t test_flag;

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function call back function
	*timer17 is  1s ,TIM3 is 0.01ms
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  static uint16_t tm0,tm3;
  static uint8_t tm1;
  
  if(htim->Instance==TIM3){
     tm3++;
     if(tm3 > 9999){
      tm3=0;
      test_flag++;
     }
     rf_irqhandler();

  }
  else if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	if(tm0 > 4){ //5s 
      tm0=0;
      tm1++;

   
       gpro_t.gTimer_run_process++;
      

      if(tm1 > 11){ //1 minute.
         tm1 =0;
        
         gpro_t.gTimer_works_total_times_minutes++ ;
        

      }

    }
    
	}
}






