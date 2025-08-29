#include "bsp.h"

uint32_t rf_data;

uint32_t rf_id_1,rf_id_2;

uint8_t checkRFCode_flag; 
volatile uint8_t id_data;
#if DEBUG
uint8_t rf_remote_syn_counter;
uint8_t rf_rec_numbers;

uint32_t rf_remote_data ;
#endif 
/*****************************************************************
*
*Function Name:void rfReceivedData_theFirst433MHZ_Handler(void)
*Function: power on has two times check receive remote code
*          two times is equal is OK
*Input Ref:
*Return Ref:
*
*****************************************************************/
void rfReceivedData_theFirst433MHZ_Handler(void)
{
 

	switch(checkRFCode_flag){

	case 0:
  
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
           rf_id_1 = g_remote_data & 0xFFFFFF;

		  #if DEBUG 
		    id_data = g_remote_data & 0x0F; //ID_DATA,20BIT is synchronization signal.
            rf_remote_data = g_remote_data ;
             
            rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
		  #endif 
		    gpro_t.rf_recieve_numbers=0;
		    gpro_t.rf_receive_data_success=0;
			gpro_t.rf_syn_signal_numbers=0;
		    g_remote_data=0;
            rf_syn_flag=0;
        }
        else{

           rf_id_2 = g_remote_data & 0xFFFFFF;

          if(rf_id_1 == rf_id_2){ //&& rf_id_1 > 0xffff){
              gpro_t.rf_decod_id = rf_id_1;
              checkRFCode_flag=1;
		  

			  #if DEBUG 
		     
              rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
              rf_remote_data = g_remote_data ;
			  gpro_t.rf_syn_signal_numbers=0;
			  #endif 
			  gpro_t.rf_recieve_numbers=0;
           //  gpro_t.powerOn_matchingId ++;
              g_remote_data=0;
              rf_syn_flag=0;
              gpro_t.rf_receive_data_success=0;
             
          }
          else{
              gpro_t.powerOn_matchingId =0;
             
              rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
              gpro_t.rf_receive_data_success=0;
              rf_remote_data = g_remote_data;
              g_remote_data =0;  
              rf_syn_flag = 0;
               gpro_t.rf_recieve_numbers =0;
               gpro_t.rf_syn_signal_numbers=0;
          }
        }
  
	break;

	case 1:


	 
	          gpro_t.power_key_flag = 1;
			  gpro_t.gTimer_switch_onoff=0;
		
			    // gpro_t.rf_receive_data_success=0;
				// gpro_t.rf_complete_receive_flag = 0;
				// gpro_t.rf_recieve_numbers=0;
                //  rf_syn_flag = 0;
		
	          
			
			  

		
	      
	   break;
	}
   
}

