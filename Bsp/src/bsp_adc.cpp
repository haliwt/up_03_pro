 #include "bsp.h"

uint32_t ADC_ConvertedValue;

uint32_t ADC_ConvertedValueLocal;

uint8_t fan_counter;


#if 0
void  fan_adc_dma_fun(void)
{

  if(gpro_t.gTimer_adc_detected_time > 7  && gpro_t.works_2_hours_timeout_flag==0 && gpro_t.fan_warning_flag ==0){

      gpro_t.gTimer_adc_detected_time=0;

     /* 启动AD转换并使能DMA传输和中断 */
      HAL_ADC_Start_DMA(&hadc1,&ADC_ConvertedValue,1);  



    /* 3.3为AD转换的参考电压值，stm32的AD转换为12bit，2^12=4096，
          即当输入为3.3V时，AD转换结果为4096 */    
       ADC_ConvertedValueLocal =(uint32_t)((ADC_ConvertedValue)*3300)/4096;   // ADC_ConvertedValue只取最低12有效数据 放大倍数1000

      if(ADC_ConvertedValueLocal< 86){
            
             fan_counter ++ ;    
         }

         if(fan_counter > 4){
          fan_counter =0;

          gpro_t.fan_warning_flag = 1;

         }

      }
  
   if(gpro_t.fan_warning_flag == 1 && gpro_t.gTimer_adc_detected_time > 3){

           gpro_t.gTimer_adc_detected_time=0;

           Voice_Warning_Sound_Fan();


   }

}

#endif 

