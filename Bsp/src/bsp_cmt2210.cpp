#include "bsp.h"


static void sync_single(void);
static void get_bits(uint8_t value,uint8_t pbuf,uint8_t sbit);

static void rf_recieve_first_data(void);
static void rf_receive_second_data(void);
static void rf_receive_first_low_level_data(void);



uint8_t byte_switch_falg,receive_byte_flag;

uint16_t low_rc,high_rc,low_sync,start_rc_flag;

uint8_t low_rc_times;

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
         gpro_t.gTime_rf_Key_low_times++;
         gpro_t.rf_auto_detected_num ++;
        // if(low_rc > 200) low_rc = 0; //200*10us = 500us 100usx 50
    }
    else if(RF_KEY_GetValue()==1 &&   (low_rc < 600 && low_rc >90) && gpro_t.g_sync_flag ==0){  //10ms 
            gpro_t.high_level_getvalue++;
            low_rc_times = gpro_t.gTime_rf_Key_low_times;

           gpro_t.g_sync_flag =1;
           gpro_t.recieve_numbers= 0;
           high_rc=0;
        
           low_rc=0;
           receive_byte_flag=0;
           gpro_t.receive_data_success=0;
            gpro_t.rf_key_interrupt_numbers=0;
            gpro_t.second_low_level_adjsut=0;

            
           
      }
      else if(RF_KEY_GetValue()==1 && gpro_t.g_sync_flag ==0){

           low_rc =0;
           gpro_t.gTime_rf_Key_low_times = 0;
          gpro_t.rf_auto_detected_num=0;
         


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
     if(gpro_t.stop_receive_data < 2){

          if((gpro_t.low_level_getvalue >92 && gpro_t.low_level_getvalue < 150) && gpro_t.stop_receive_data==1){

                gpro_t.recieve_numbers=0;
 
                gpro_t.low_level_getvalue=0;
                gpro_t.high_level_getvalue=0;
                receive_byte_flag=0;
                gpro_t.second_low_level_adjsut=1;

                rc_l++;
               

                return;


          }
          else if((gpro_t.low_level_getvalue >2 && gpro_t.low_level_getvalue < 150)&& gpro_t.stop_receive_data  !=2){
          if(rc_h_num != rc_h){
           rc_h_num = rc_h;
           rc_l++;
           gpro_t.recieve_numbers++;
 
          }

         if(gpro_t.low_level_getvalue <= 7 && gpro_t.low_level_getvalue >2){ //get numbers 1; 

           if(gpro_t.stop_receive_data==0 ){
                rf_recieve_first_data();
             
            }
            else if(gpro_t.stop_receive_data==1){

                  rf_receive_second_data();
            }
          }
          else if(gpro_t.low_level_getvalue > 7 && gpro_t.low_level_getvalue < 150){ //get number "0 "
         
           
              rf_receive_first_low_level_data();

            
          }
          
        }
        else if(gpro_t.low_level_getvalue > 600 || gpro_t.stop_receive_data  ==2){
              
                gpro_t.recieve_numbers=0;
 
                gpro_t.low_level_getvalue=0;
                gpro_t.high_level_getvalue=0;
                gpro_t.rf_rec_data2=0;
                gpro_t.rf_rec_data2_2 =0;
                receive_byte_flag=0;
                 gpro_t.stop_receive_data  =0;

                  gpro_t.g_sync_flag=0;
                  return ;

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
/**************************************************************
*
*Function Name:
*Function:
*Input Ref:
*Return Ref:
*
***************************************************************/
static void rf_recieve_first_data(void)
{
    if(receive_byte_flag==0){
     switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data= 0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data= 0x40;  //
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data=0x20; 
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data= 0x10; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data=0x08; 
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data= 0x04; 
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data= 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					    gpro_t.rf_rec_data= 0x01; 
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
                        
                        gpro_t.rf_rec_data1= 0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						gpro_t.rf_rec_data1= 0x40;
						break; 
					}
					case 3 : 
					{ 
						gpro_t.rf_rec_data1= 0x20;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						gpro_t.rf_rec_data1= 0x10;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						gpro_t.rf_rec_data1= 0x08;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data1= 0x04;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data1= 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data1= 0x01;
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
              
              
              if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                 receive_byte_flag=2;
               }
               gpro_t.low_level_getvalue=0;//gpro_t.recieve_numbers ++;
           }
           else if(receive_byte_flag==2){
                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                       
                        gpro_t.rf_rec_data2 = 0x80; //最高位 
                        
                      
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data2 =0x40;
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x20;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x10;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data2 = 0x08;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x04;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data2 = 0x01; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
                
               gpro_t.low_level_getvalue=0;//gpro_t.recieve_numbers ++;
               if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
               
                gpro_t.stop_receive_data++;
                
                gpro_t.gTime_rf_rc_data=0;
             
                //gpro_t.high_level_getvalue=0;
              
             
               }
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
               low_rc=0;
         
            }


}
/**************************************************************
*
*Function Name:void rf_receive_first_low_level_data(void)
*Function:
*Input Ref:
*Return Ref:
*
***************************************************************/
void rf_receive_first_low_level_data(void)
{
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
         else if(receive_byte_flag==2 &&  gpro_t.second_low_level_adjsut==0){
            
            
             if(gpro_t.recieve_numbers ==8){
              gpro_t.recieve_numbers =0;
              receive_byte_flag=0;
           
              gpro_t.stop_receive_data++;
              }
             gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
         
         
         
          }
          else if(receive_byte_flag==2 &&  gpro_t.second_low_level_adjsut==1){
            
            
             if(gpro_t.recieve_numbers ==8){
              gpro_t.recieve_numbers =0;
              receive_byte_flag=0;
           
               gpro_t.stop_receive_data++;
               if(gpro_t.stop_receive_data > 1){
                    gpro_t.receive_data_success=1;
                    gpro_t.gTimer_stop_receive=0;

                }  

             
              low_rc=0;
           
              }
             gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
             low_rc=0;
         
         
          }
 }
/**************************************************************
*
*Function Name:
*Function:
*Input Ref:
*Return Ref:
*
***************************************************************/
static void rf_receive_second_data(void)
{

     if(receive_byte_flag==0){

             
               switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data_2= 0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data_2= 0x40;  //
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data_2=0x20; 
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data_2= 0x10; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data_2= 0x08; 
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data_2= 0x04; 
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data_2= 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					    gpro_t.rf_rec_data_2= 0x01; 
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
                        
                        gpro_t.rf_rec_data1_2= 0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x40;
						break; 
					}
					case 3 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x20;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x10;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						gpro_t.rf_rec_data1_2= 0x08;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x04;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data1_2= 0x01;
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
              
              
              if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                 receive_byte_flag=2;
               }
               gpro_t.low_level_getvalue=0;//gpro_t.recieve_numbers ++;
           }
           else if(receive_byte_flag==2){
                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                       
                        gpro_t.rf_rec_data2_2 = 0x80; //最高位 
                        
                      
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x40;
						break; 
					}
					case 3 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x20;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x10;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						  gpro_t.rf_rec_data2_2 = 0x08;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x04;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x02;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					      gpro_t.rf_rec_data2_2 = 0x01; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
                
             
              if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
               
                gpro_t.stop_receive_data++;
                if(gpro_t.stop_receive_data > 1){
                    gpro_t.receive_data_success=1;
                    gpro_t.gTimer_stop_receive=0;

                }  
                gpro_t.gTime_rf_rc_data=0;

                low_rc=0;
              
               }
               gpro_t.low_level_getvalue=0;//gpro_t.high_level_getvalue=0;
               low_rc=0;
         
            }
}






