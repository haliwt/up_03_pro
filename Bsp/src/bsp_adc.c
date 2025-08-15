 #include "bsp.h"


uint8_t fan_counter ;


void  fan_adc_dma_fun(void)
{

  
      read_adc_dma_voltage();

      
      if(gdma_voltage[0]< 86){
            
             fan_counter ++ ;    
      }else if(gdma_voltage[0] > 86){

            fan_counter=0;

      }

        if(fan_counter > 4){
          fan_counter =0;

          gpro_t.fan_warning_flag = 1;

         }

    
  
   if(gpro_t.fan_warning_flag == 1){

           
          // Voice_Warning_Sound_Fan();
   }
 

}



