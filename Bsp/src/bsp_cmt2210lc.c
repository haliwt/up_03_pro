#include "bsp.h"

uint32_t rf_data;

uint8_t rf_rec_numbers;

uint32_t rf_id_1,rf_id_2;

uint8_t checkRFCode_flag;


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



   if(checkRFCode_flag==0){
        gpro_t.powerOn_matchingId++;
        if(gpro_t.powerOn_matchingId ==1){
          rf_id_1 = g_remote_data & 0x07FFFFFF;
        }
        else{

           rf_id_2 = g_remote_data & 0x07FFFFFF;

          if(rf_id_1 == rf_id_2 && rf_id_1 > 0xffff){
              gpro_t.rf_id = rf_id_1;
              checkRFCode_flag=1;
              gpro_t.powerOn_matchingId =0;

          }
          else{
              gpro_t.powerOn_matchingId =0;


          }
        }
    }
  

   // rf_data= g_remote_data;
    rf_data = g_remote_data & 0x07FFFFFF;

    if(rf_data == gpro_t.rf_id && checkRFCode_flag ==1){
        rf_rec_numbers =  gpro_t.rf_recieve_numbers;
        gpro_t.rf_receive_data_success++; //gpro_t.rf_receive_data_success=3;
        gpro_t.rfPowerOnOff_soundFLag =1;
        g_remote_data =0;
        rf_syn_flag = 0;
        gpro_t.rf_syn_signal_numbers=0;
        gpro_t.rf_recieve_numbers=0;
       
       

    }
    else{
        gpro_t.rf_recieve_numbers =0;
        gpro_t.rf_receive_data_success=0;
        rf_syn_flag = 0;
       gpro_t.rf_syn_signal_numbers=0;
       g_remote_data =0;
      


    }

   
}

