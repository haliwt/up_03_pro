#include "bsp.h"



uint32_t rf_id_1,rf_id_2;

uint8_t checkRFCode_flag; 


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
 

	if(checkRFCode_flag==0){

  
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
           rf_id_1 = g_remote_data & 0xFFFFFF ;

		 
		    gpro_t.rf_recieve_numbers=0;
		    gpro_t.rf_receive_data_success=0;
			 
		    g_remote_data=0;
        rf_syn_flag=0;
        }
        else{

           rf_id_2 = g_remote_data & 0xFFFFFF  ;

          if(rf_id_1 == rf_id_2){ //&& rf_id_1 > 0xffff){
              gpro_t.rf_decod_id = rf_id_1;
              gpro_t.rf_decod_id = gpro_t.rf_decod_id & 0xFFFFFF ;
              checkRFCode_flag=1;
		        // gpro_t.power_key_flag = 1;
             gpro_t.rf_complete_receive_flag = 1;
             
              gpro_t.rfPowerOnOff_soundFLag =1;
              g_remote_data=0;
             
             
          }
          else{
              gpro_t.powerOn_matchingId =0;
              gpro_t.rf_receive_data_success=0;
          
              g_remote_data =0;  
              rf_syn_flag = 0;
               gpro_t.rf_recieve_numbers =0;
              
          }
        }
  
  }


}

