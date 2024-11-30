#include "bsp.h"




/***************************************************************************
 *  *
    *Function Name: void main_board_ctl_handler(uint8_t flag,uint8_t cmd)
    *Function: flag: 休息10分钟， 还是在工作2个小时内
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
****************************************************************************/
void main_board_ctl_handler(uint8_t flag,uint8_t cmd)
{
    //TODO
   
    if(flag == 0){ //works 2 hours times lable .
        switch(cmd){

        case ai_mode: //0x0
            if(gpro_t.gTimer_normal_run_main_function_times > 6){
                 gpro_t.gTimer_normal_run_main_function_times=0;
                fan_output_fun();
                plasma_output_fun();
                ultra_output_fun();

            }
            led_on_fun();
        break;

        case timer_mode: //0x01

           if(gpro_t.gTimer_normal_run_main_function_times > 6){
               gpro_t.gTimer_normal_run_main_function_times=0;
               fan_output_fun();
               plasma_output_fun();
               ultra_output_fun();

              }

            if(gpro_t.gTimer_set_minutes > 59){ // 59 one hours = 60 minutes
                 gpro_t.gTimer_set_minutes=0;

             #if UNIT_TESTING

                gpro_t.set_timer_timing_value --;


               #else
                  gpro_t.set_timer_timing_value--;

               #endif

              if(gpro_t.set_timer_timing_value==0 ){

                    gpro_t.power_on = power_off; 


              }
              
			
			}

            
            led_on_fun();
            osDelay(1000);
            led_off_fun();
            osDelay(1000);

        break;

        }
    }
    else {
        
        
         fan_stop_fun();
         plasma_stop_fun();
         ultra_stop_fun();


    }
}



