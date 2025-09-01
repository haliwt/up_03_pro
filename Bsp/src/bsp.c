#include "bsp.h"

PROCESS_T  gpro_t;
/************************************************************************************************
*
*Function Name:void bsp_init(void)
*Function: 
*Input Ref:
*Return Ref:
*
**************************************************************************************************/
void bsp_init(void)
{
   DMA_ADC_Init();
#if Enable_EventRecorder == 1  
	/* 𸀆𸀽𸀜𸀎𸀍𸀄EventRecorder𸀅𷿺𸀑𷿿𸀘𸀼 */
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
	

}



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

   if(gpro_t.rfPowerOnOff_soundFLag==1 && gpro_t.power_on == power_off){

   
    
		gpro_t.power_on = power_on;
        
		gpro_t.rfPowerOnOff_soundFLag=3;
      
	    gpro_t.works_2_hours_timeout_flag=0;
        gpro_t.fan_warning_flag = 0;
        gpro_t.gTimer_normal_run_main_function_times =10;
        gdma_voltage[0] = 0; //WT.EDIT 2025.05.14
        led_on_fun();
        voice_power_on_sound();
        printf("power_on !!!\r\n");
      
    
		
    }
   else if(gpro_t.rfPowerOnOff_soundFLag==1 && gpro_t.power_on == power_on){

      
        gpro_t.power_on = power_off;
		gpro_t.rfPowerOnOff_soundFLag=4;
        led_off_fun();
        voice_power_off_sound();
        printf("power_off !!!\r\n");
        
    }
   
    	   

}
/*****************************************************************
*
*Function Name:void powerOnOff_handler(void)
*Function: 
*Input Ref:
*Return Ref:
*
*****************************************************************/
void powerOnOff_handler(void)
{
	if(gpro_t.power_on == power_off){

	    gpro_t.rfPowerOnOff_soundFLag =1;
       
	}
	else if(gpro_t.power_on == power_on){

	  gpro_t.rfPowerOnOff_soundFLag =1;

	}
	

}

