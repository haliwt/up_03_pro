#include "bsp.h"
//#include "bap_fan.h"

PROCESS_T  gpro_t;

 fan_class fan_t;
 plasma_class plasma_t;
ultra_class ultra_t;


void power_off_handler(void)
{
    // Turn off the device
    // ...
   
    
    
    fan_t.fan_stop_fun();
    
    plasma_t.plasma_stop_fun();
    ultra_t.ultra_stop_fun();
     gpro_t.gTimer_works_total_times_minutes=0;
    
    }


void device_works_time_counter_handler(void)
{

    if(gpro_t.works_2_hours_timeout_flag==0){

        if(gpro_t.gTimer_works_total_times_minutes >119){//119  //  //2 hours 120 minutes.
           gpro_t.gTimer_works_total_times_minutes=0;
            gpro_t.works_2_hours_timeout_flag = 1;  
        }
    }
    else{
        if(gpro_t.gTimer_works_total_times_minutes >10){ //10

             gpro_t.gTimer_works_total_times_minutes= 0; 
         
             gpro_t.works_2_hours_timeout_flag = 0;  
         }
    }
}


