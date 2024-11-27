#include "bsp.h"

uint8_t  rf_auto_detected_num;

void HAL_GPIO_EXTI_Falling_Callback(GPIO_Pin)
{

   if(GPIO_Pin == RF_KEY_Pin){

       
        if(gpro_t.g_sync_flag == 1){
            

          gpro_t.rf_key_interrupt_numbers++ ;
          rf_auto_detected_num=0;
        }
        else{

           rf_auto_detected_num++;
           gpro_t.rf_key_interrupt_numbers=0;

        }


   }




}


/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function call back function
	*timer17 is  1s ,TIM3 is 0.01ms
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  static uint16_t tm0;
  static uint8_t tm1;
  
  if(htim->Instance==TIM3){
  
     rf_irqhandler();

  }
  else if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	if(tm0 > 0){ //5s 
      tm0=0;
      tm1++;

      gpro_t.gTime_rf_rc_data++;
       gpro_t.gTimer_run_process++;
      

      if(tm1 > 11){ //1 minute.
         tm1 =0;
        
         gpro_t.gTimer_works_total_times_minutes++ ;
        

      }

    }
    
	}
}






