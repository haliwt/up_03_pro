#include "bsp.h"


static void sync_single(void);


uint8_t sync_start_flag;
uint8_t decode_ok;            //解码成功
uint16_t  high_getvalue,low_getvalue;           //高,低电平宽度
uint8_t  receive_decoder_numbers;//ma_x;                //接收到第几位编码了




uint8_t rf_data[4];

uint16_t low_rc,high_rc,low_sync,start_rc_flag;


/***************************************************
*
* TIM3 timer is 10us 
**
***************************************************/
static void sync_single(void)
{
    
   // static uint8_t low_rc,high_rc,low_sync,start_rc_flag;
   static uint8_t low_start_flag,high_end_flag,low_receive_flag;
  
   if(RF_KEY_GetValue()==0   && sync_start_flag ==0){
         
          high_rc=0;
         low_rc++;
        // if(low_rc > 200) low_rc = 0; //200*10us = 500us


    }
    if(RF_KEY_GetValue()==1 &&   (low_rc <= 1500 && low_rc >700) && sync_start_flag ==0){
             high_end_flag=2;
               

          

                sync_flag =1;
                sync_start_flag =1;
                receive_decoder_numbers= 0;
                bma1=0;
                bma2=0;
                bma3=0;
                bma4=0;

             
               low_level_rc = 0; 
              high_rc=0;


      }
      else if(RF_KEY_GetValue()==1 &&   (low_rc > 1500 || low_rc  < 700) && sync_start_flag ==0){

         low_rc =0;


      }

     
     if(RF_KEY_GetValue()==1   && sync_start_flag ==1){

                  high_rc++ ;
                if(high_rc > 2000){
                sync_start_flag=0;
                 sync_flag =0;

                low_rc=0,
                high_rc=0;
               low_start_flag=0;
               high_end_flag=0;
               low_sync=0;
                start_rc_flag=0;

                    }
            

     }
  
 }


void rf_irqhandler(void)
{
 //  RF_KEY_GetValue();

   sync_single();

   
 }




