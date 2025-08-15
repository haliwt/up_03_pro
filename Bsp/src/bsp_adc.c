 #include "bsp.h"


uint8_t fan_counter ;


void  fan_adc_dma_fun(void)
{

     static uint16_t adc_timer_counter;
      adc_timer_counter++;
      read_adc_dma_voltage();

      
      if(gdma_voltage[0]< 86){
            
             fan_counter ++ ;    
         }

         if(fan_counter > 4){
          fan_counter =0;

          gpro_t.fan_warning_flag = 1;

         }

    
  
   if(gpro_t.fan_warning_flag == 1 &&  adc_timer_counter > 5000){

            adc_timer_counter++;
          // Voice_Warning_Sound_Fan();
   }
   else if(gpro_t.fan_warning_flag == 1 &&  adc_timer_counter < 5000){
              adc_timer_counter++;

   }


}



