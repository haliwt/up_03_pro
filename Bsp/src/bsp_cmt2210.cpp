#include "bsp.h"


static void sync_single(void);
static void get_bits(uint8_t value,uint8_t pbuf,uint8_t sbit);

uint8_t byte_switch_falg,receive_byte_flag;

uint16_t low_rc,high_rc,low_sync,start_rc_flag;


/***************************************************
*
* TIM3 timer is 10us 
**
***************************************************/
static void sync_single(void)
{
    
   // static uint8_t low_rc,high_rc,low_sync,start_rc_flag;

 
   if(RF_KEY_GetValue()==0 && gpro_t.g_sync_flag ==0){
         
          high_rc=0;
         low_rc++;
        // if(low_rc > 200) low_rc = 0; //200*10us = 500us 100usx 50


    }
    else if(RF_KEY_GetValue()==1 &&   (low_rc < 120 && low_rc >92) && gpro_t.g_sync_flag ==0){  //10ms 
            gpro_t.high_level_getvalue++;
      
           gpro_t.g_sync_flag =1;
           gpro_t.recieve_numbers= 0;
  
            high_rc=0;
        
           low_rc=0;
           gpro_t.receive_data_success=0;
      }
      else if(RF_KEY_GetValue()==1 && gpro_t.g_sync_flag ==0){

         low_rc =0;


      }
     
      
     
    

    
    
  
 }


void rf_irqhandler(void)
{
 //  RF_KEY_GetValue();
   static uint8_t rc_l_num= 0xff,rc_h_num=0xff,rc_l,rc_h;
   switch(gpro_t.g_sync_flag){

   case 0:
    
     sync_single();

   break;

   case 1:

  
   if(RF_KEY_GetValue()==0){ //
       
       gpro_t.high_level_getvalue=0;
       gpro_t.low_level_getvalue ++;
       if(rc_l_num != rc_l){
           rc_l_num = rc_l;
           rc_h++;
 
       }

   }
   else if(RF_KEY_GetValue()==1){

     
     gpro_t.high_level_getvalue++; //gpro_t.low_level_getvalue ++; 
     if(gpro_t.stop_receive_data ==0){
     
          if(rc_h_num != rc_h){
           rc_h_num = rc_h;
           rc_l++;
           gpro_t.recieve_numbers++;
 
          }

         if(gpro_t.low_level_getvalue < 4){ //get numbers 0; 

           if(receive_byte_flag==0){
               //get_bits(0x0,rf_data[0],gpro_t.recieve_numbers);
               switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data= 1<< 7; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data= 1 << 6;  //
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data=1<<5; 
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data= 1<<4; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data=1<<3; 
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data= 1<<2; 
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data= 1<< 1;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					    gpro_t.rf_rec_data= 1<<0; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
               
               if(gpro_t.recieve_numbers ==8){
                  gpro_t.recieve_numbers =0;
                   receive_byte_flag=1;
                }
               gpro_t.low_level_getvalue=0;// gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==1){

                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data1= 1<< 7; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						gpro_t.rf_rec_data1= 1<< 6;
						break; 
					}
					case 3 : 
					{ 
						gpro_t.rf_rec_data1= 1<< 5;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						gpro_t.rf_rec_data1= 1<< 4;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						gpro_t.rf_rec_data1= 1<< 3;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data1= 1<< 2;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data1= 1<< 1;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data1= 1<< 0;
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
              
               gpro_t.low_level_getvalue=0;//gpro_t.recieve_numbers ++;
              if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=2;
               }
              gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==2){
                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data2 = 1<< 7; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data2 = 1<< 6;
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data2 = 1<< 5;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data2 = 1<< 4;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data2 = 1<< 3;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data2 = 1<< 2;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data2 = 1<< 1;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data2 = 1<< 0; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
               gpro_t.low_level_getvalue=0;//gpro_t.recieve_numbers ++;
               if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
                gpro_t.receive_data_success=1;
                gpro_t.stop_receive_data++;
                    
                gpro_t.gTime_rf_rc_data=0;

                low_rc=0;
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
                gpro_t.high_level_getvalue=0;
                 gpro_t.low_level_getvalue =0;
             
               
                
               }
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
               low_rc=0;
           }
          
        }
        else{ //get number "1"
           if(receive_byte_flag==0){
              
               
                if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=1;
                }
                gpro_t.low_level_getvalue=0;// gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==1){
               
                
                  if(gpro_t.recieve_numbers ==8){
                   gpro_t.recieve_numbers =0;
                   receive_byte_flag=2;
                   }
                 gpro_t.low_level_getvalue=0;//  gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==2){
              
              
               if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
                gpro_t.receive_data_success =1 ;
                
              
                gpro_t.gTime_rf_rc_data=0;

               low_rc=0;
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
                gpro_t.high_level_getvalue=0;
                gpro_t.low_level_getvalue =0;
           
               
                
               }
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
               low_rc=0;
           
           
            }

          }
        }
        else if(gpro_t.stop_receive_data   > 1 ){
                   
              if( gpro_t.low_level_getvalue > 1000 ){

               //  gpro_t.receive_data_success = 0 ; 

                gpro_t.g_sync_flag=0;
                low_rc=0;
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
               gpro_t.stop_receive_data =0; 
              gpro_t.recieve_numbers=0;

       
              
              }
              else{
                gpro_t.recieve_numbers=0;
                gpro_t.stop_receive_data =0;

                gpro_t.low_level_getvalue=0;
              //   gpro_t.receive_data_success = 0 ; 

                gpro_t.g_sync_flag=0;
                low_rc=0;
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;

              }
      
        }
       
    }
     break;
   }
 }
 


static void get_bits(uint8_t value,uint8_t pbuf,uint8_t sbit)
{

   pbuf = (value << sbit) & 0x01;

   

}


