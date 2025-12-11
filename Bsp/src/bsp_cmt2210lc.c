#include "bsp.h"



uint32_t rf_id_1,rf_id_2;

uint8_t pair_remote_ID_success_flag; 


/*****************************************************************
*
*Function Name:void remoteControl_ID_firstPair433Mhz_handler(void)
*Function: power on has two times check receive remote code
*          two times is equal is OK
*Input Ref:
*Return Ref:
*
*****************************************************************/
void remoteControl_ID_firstPair433Mhz_handler(void)
{
 

	if(pair_remote_ID_success_flag==0){

  
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
           rf_id_1 = g_remote_data & 0xFFFFFF ;

		 
		    gpro_t.rf_rx_data_num=0;
		    gpro_t.rf_receive_data_success=0;
			 
		    g_remote_data=0;
            rf_sync_signal_flag=0;
        }
        else{

           rf_id_2 = g_remote_data & 0xFFFFFF  ;

          if(rf_id_1 == rf_id_2){ //&& rf_id_1 > 0xffff){
              gpro_t.rf_decod_ID = rf_id_1;
         
              pair_remote_ID_success_flag=1;
		         
             gpro_t.rf_complete_receive_flag = 1;
             gpro_t.rfPowerOnOff_soundFLag =1;
             
              gpro_t.gTimer_rf_receive_counter=0;
             
             
             
          }
          else{
              gpro_t.powerOn_matchingId =0;
              gpro_t.rf_receive_data_success=0;
              g_remote_data =0;  
              rf_sync_signal_flag = 0;
              gpro_t.rf_rx_data_num =0;
              
          }
        }
  
  }


}

