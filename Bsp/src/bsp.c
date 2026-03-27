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
   gpro_t.power_on = power_off;
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
    gpro_t.power_on = power_off;
    led_off_fun();
    fan_stop_fun();
    
     plasma_stop_fun();
     ultra_stop_fun();
     gpro_t.works_2_hours_timeout_flag=0;
     gpro_t.fan_warning_flag = 0;
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
             fan_output_fun();  
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

  if(gpro_t.rfPowerOnOff_soundFLag==2 || gpro_t.rfPowerOnOff_soundFLag==0) return ;
   
  
   if(gpro_t.rfPowerOnOff_soundFLag==1 && gpro_t.power_on == power_off){


        if(gpro_t.dc_power_on_first==1 || gpro_t.dc_power_on_first ==0){ //WT.EDIT 2025.11.10
			gpro_t.dc_power_on_first=2;
			gpro_t.rfPowerOnOff_soundFLag=2;
		 	rf_sync_signal_flag = 0; //WT.EDIT 2025.12.10
	      	gpro_t.rf_complete_receive_flag = 0;
	      	gpro_t.rf_rx_data_num=0;
	      
	     	gpro_t.rfPowerOnOff_soundFLag=0; 
		    tx_thread_sleep(300);//vTaskDelay(pdMS_TO_TICKS(300));//WT.EDIT 2025.12.10 //vTaskDelay(pdMS_TO_TICKS(1000));
			gpro_t.rf_receive_data_success=0; //WT.EDIT 2025.12.10 add .
		    
		}
		else{
       
        
		gpro_t.rfPowerOnOff_soundFLag++;
         gpro_t.power_on = power_on;
	    gpro_t.works_2_hours_timeout_flag=0;
        gpro_t.fan_warning_flag = 0;
        gpro_t.gTimer_normal_run_main_function_times =10;
       // gdma_voltage[0] = 0; //WT.EDIT 2025.05.14
        led_on_fun();
        voice_power_on_sound();
		#if DEBUG
        	printf("power_on !!!\r\n");
		#endif 
	
	     rf_sync_signal_flag = 0;//1 //WT.EDIT 2025.12.10 modify 
	     gpro_t.rfPowerOnOff_soundFLag=0; 
		 gpro_t.rf_rx_data_num=0;
         tx_thread_sleep(500);//vTaskDelay(pdMS_TO_TICKS(500));
	    //rf_sync_signal_flag = 0;
	     gpro_t.rf_receive_data_success=0; //WT.EDIT 2025.12.10 add .
	
    }
   }
   else if(gpro_t.rfPowerOnOff_soundFLag==1 && gpro_t.power_on == power_on){

      	gpro_t.rfPowerOnOff_soundFLag++;
        gpro_t.power_on = power_off;
	
        led_off_fun();
        voice_power_off_sound();
	
		#if DEBUG
        	printf("power_off !!!\r\n");
		#endif 

       gpro_t.rfPowerOnOff_soundFLag=2;
		
	       rf_sync_signal_flag = 0; //WT.EDIT modify 2025.12.10

			gpro_t.rf_rx_data_num=0;
	     	gpro_t.rfPowerOnOff_soundFLag=0; 
		  tx_thread_sleep(300);//vTaskDelay(pdMS_TO_TICKS(300));//WT.EDIT modify :2025.12.10//vTaskDelay(500);
		  //rf_sync_signal_flag = 0;
		  gpro_t.rf_receive_data_success=0; //WT.EDIT 2025.12.10 add .

	}
         
 }


