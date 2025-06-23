#include "bsp.h"

PROCESS_T  gpro_t;

//uint32_t rf_data;


//uint8_t rf_rec_numbers;

//uint32_t rf_id_1,rf_id_2;

//uint8_t checkRFCode_flag;


/*****************************************************************
*
*Function Name:void power_off_handler(void)
*Function: 
*Input Ref:
*Return Ref:
*
*****************************************************************/
void power_off_handler(void)
{
    // Turn off the device
    // ...
    led_off_fun();
    fan_stop_fun();
    
     plasma_stop_fun();
     ultra_stop_fun();
     gpro_t.gTimer_works_total_times_minutes=0;
    
    
    
}
/*****************************************************************
*
*Function Name:void device_works_time_counter_handler(void)
*Function: 
*Input Ref:
*Return Ref:
*
*****************************************************************/
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
             gpro_t.gTimer_normal_run_main_function_times =10;
             gpro_t.works_2_hours_timeout_flag = 0;  
         }
    }
}



/*****************************************************************
*
*Function Name:void sound_power_on_off_handler(void)
*Function: 
*Input Ref:
*Return Ref:
*
*****************************************************************/
void sound_power_on_off_handler(void)
{

   if((gpro_t.power_on_off_numbers==1 && gpro_t.gTimer_switch_onoff > 2 && gpro_t.power_key_flag==2) 
           ||(gpro_t.power_on_off_numbers==power_on && gpro_t.gTimer_switch_onoff > 0 &&  gpro_t.rfPowerOnOff_soundFLag==2)){
        gpro_t.power_key_flag=3;
        gpro_t.rfPowerOnOff_soundFLag=3;
        gpro_t.power_on_off_numbers++;
        gpro_t.gTimer_switch_onoff=0;
        gpro_t.power_on = power_on;

        gpro_t.works_2_hours_timeout_flag=0;
        gpro_t.fan_warning_flag = 0;
        gpro_t.gTimer_normal_run_main_function_times =10;

        led_on_fun();
        voice_power_on_sound();
        gpro_t.gTimer_power_on_times=0;
        rf_syn_flag = 0;
        gpro_t.rf_syn_signal_numbers=0;
        g_remote_data =0;


  }
  else if((gpro_t.power_on_off_numbers == 3 && gpro_t.gTimer_switch_onoff > 2 && gpro_t.power_key_flag==2  )
            ||(gpro_t.power_on_off_numbers == 3 && gpro_t.gTimer_switch_onoff > 0  && gpro_t.rfPowerOnOff_soundFLag==2)){
        gpro_t.power_key_flag=3;
        gpro_t.rfPowerOnOff_soundFLag=3;
        gpro_t.power_on_off_numbers++;
        gpro_t.gTimer_switch_onoff=0;
        gpro_t.power_on = power_off;
        led_off_fun();
        voice_power_off_sound();
        gpro_t.gTimer_power_on_times=0;
        rf_syn_flag = 0;
        gpro_t.rf_syn_signal_numbers=0;
        g_remote_data =0;

  }
   
 

}

