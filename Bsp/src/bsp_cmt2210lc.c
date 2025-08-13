#include "bsp.h"

uint32_t rf_data;

uint8_t rf_rec_numbers;

uint32_t rf_id_1,rf_id_2;

uint8_t checkRFCode_flag, rf_remote_syn_counter;

uint32_t rf_remote_data ;
/*****************************************************************
*
*Function Name:void rfReceivedData_Handler(void)
*Function: power on has two times check receive remote code
*          two times is equal is OK
*Input Ref:
*Return Ref:
*
*****************************************************************/
void rfReceivedData_Handler(void)
{
    static uint8_t error_counter;
    if(checkRFCode_flag==0){
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
           rf_id_1 = g_remote_data & 0xFFFFFF;
           rf_remote_data = g_remote_data;
              g_remote_data=0;
              rf_syn_flag=0;
            rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
		   gpro_t.rf_recieve_numbers=0;
		   gpro_t.rf_receive_data_success=0;
			gpro_t.rf_syn_signal_numbers=0;
        }
        else{

           rf_id_2 = g_remote_data & 0xFFFFFF;

          if(rf_id_1 == rf_id_2){ //&& rf_id_1 > 0xffff){
              gpro_t.rf_decod_id = rf_id_1;
              checkRFCode_flag=1;
		      gpro_t.rf_recieve_numbers=0;
              gpro_t.powerOn_matchingId =0;
              rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
              rf_remote_data = g_remote_data;
              g_remote_data=0;
              rf_syn_flag=0;
              gpro_t.rf_receive_data_success=0;
              gpro_t.rf_syn_signal_numbers=0;
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
    }
    else if(checkRFCode_flag==1){

	   // rf_data= g_remote_data,433MHZ,protocl 1527,ID=20BIT ,24BIT;
	    	rf_data = g_remote_data & 0xFFFFFF;

	    if(rf_data == gpro_t.rf_decod_id ){
	        rf_rec_numbers =  gpro_t.rf_recieve_numbers;
	        gpro_t.rf_receive_data_success=0; //gpro_t.rf_receive_data_success=3;
	        gpro_t.rfPowerOnOff_soundFLag =1;
            rf_remote_data = g_remote_data;
            rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
	        g_remote_data =0;
	        rf_syn_flag = 0;
	        gpro_t.rf_syn_signal_numbers=0;
           
	        gpro_t.rf_recieve_numbers=0;
	        
	       

	    }
		else{
			  printf("receive_num = %d\r\n",gpro_t.rf_recieve_numbers);
              gpro_t.rf_receive_data_success=0;
			  gpro_t.rf_recieve_numbers =0;
			  rf_syn_flag=0;
              rf_remote_syn_counter = gpro_t.rf_syn_signal_numbers;
			  printf("syn_conter = %d\r\n",rf_remote_syn_counter);
			   rf_remote_data = g_remote_data;
			   gpro_t.rf_syn_signal_numbers=0;
			   g_remote_data=0;
               
			   gpro_t.rf_syn_signal_numbers=0;
			   //error_counter++;
			   //printf("error = %d\r\n",error_counter);

		}
	}
   
   
}

