#include "bsp.h"

PROCESS_T  gpro_t;

uint32_t rf_data;


uint8_t rf_rec_numbers;

uint32_t rf_id_1,rf_id_2;




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
*Function Name:void rfReceivedData_Handler(void)
*Function: 
*Input Ref:
*Return Ref:
*
*****************************************************************/
void rfReceivedData_Handler(void)
{
   if(gpro_t.powerOn_matchingId < 10){
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
          rf_id_1 = g_remote_data & 0x07FFFFFF;
        }
        else{

           rf_id_2 = g_remote_data & 0x07FFFFFF;

          if(rf_id_1 == rf_id_2){
              gpro_t.rf_id = rf_id_1;
              gpro_t.powerOn_matchingId =11;

          }
          else{
              gpro_t.powerOn_matchingId =0;


          }


        }


    }
  

   // rf_data= g_remote_data;
    rf_data = g_remote_data & 0x07FFFFFF;

    if(rf_data == gpro_t.rf_id){
        rf_rec_numbers =  gpro_t.rf_recieve_numbers;
        gpro_t.rf_receive_data_success++;
        gpro_t.power_onoff_sound_flag =1;
        g_remote_data =0;
        gpro_t.rf_recieve_numbers =0;

    }
    else{
        gpro_t.rf_recieve_numbers =0;
        gpro_t.rf_receive_data_success=0;
    


    }

   
}


