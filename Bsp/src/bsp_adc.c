 #include "bsp.h"





void  fan_adc_dma_fun(void)
{

      
      read_adc_dma_voltage();
       if(gpro_t.fan_warning_flag == 1){

        fan_fault_handler();
           
      }
  

}



