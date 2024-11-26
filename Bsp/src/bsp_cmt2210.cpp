#include "bsp.h"


static void sync_single(void);
static void get_bits(uint8_t value,uint8_t pbuf,uint8_t sbit);





uint8_t rf_rec_data,rf_rec_data1,rf_rec_data2;

uint8_t sync_start_flag,receive_first_bit_flag ;
uint8_t decode_ok;            //解码成功
uint16_t  high_getvalue,low_getvalue;           //高,低电平宽度
uint8_t  receive_numbers;//ma_x;                //接收到第几位编码了

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

 
   if(RF_KEY_GetValue()==0){
         
          high_rc=0;
         low_rc++;
        // if(low_rc > 200) low_rc = 0; //200*10us = 500us 100usx 50


    }
    if(RF_KEY_GetValue()==1 &&   (low_rc < 1500 && low_rc >850)){
            high_getvalue++;
           sync_start_flag =1;
           gpro_t.g_sync_flag =1;
           receive_numbers= 0;
           receive_first_bit_flag =1;
            high_rc=0;
           gpro_t.gTime_rf_rc_data =0 ;
           low_rc=0;
      }
      else if(RF_KEY_GetValue()==1 &&   (low_rc  < 850 || low_rc > 1500)){

         low_rc =0;


      }
     
    

    
    
  
 }


void rf_irqhandler(void)
{
 //  RF_KEY_GetValue();
   switch(gpro_t.g_sync_flag){

   case 0:
    
      sync_single();

     
//     else{
//
//        if(RF_KEY_GetValue()==1 && gpro_t.gTime_rf_rc_data > 8    && gpro_t.stop_receive_data && gpro_t.g_sync_flag==1 ){
//            gpro_t.gTime_rf_rc_data=0;
//           sync_start_flag =0;
//           receive_numbers= 0;
//           receive_first_bit_flag =0;
//           gpro_t.stop_receive_data=0;
//           gpro_t.receive_data_success=0;
//          high_rc=0; 
//           low_rc =0;
//
//       }
//
//     }

   break;

   case 1:
   if(RF_KEY_GetValue()==1 && gpro_t.gTime_rf_rc_data > 6 && gpro_t.g_sync_flag==1 ){
              gpro_t.gTime_rf_rc_data=0;
             sync_start_flag =0;
             receive_numbers= 0;
             receive_first_bit_flag =0;
             gpro_t.stop_receive_data=0;
             gpro_t.receive_data_success=0;
             gpro_t.g_sync_flag=0;
            high_rc=0; 
             low_rc =0;
   
      }


  // break;

   case 5:
    
   if(RF_KEY_GetValue()==0 &&   receive_first_bit_flag ==0){ //
        gpro_t.gTime_rf_rc_data=0;
        high_getvalue=0;
       low_getvalue ++;

   }
   else if(RF_KEY_GetValue()==1 &&   sync_start_flag ==1){
       gpro_t.gTime_rf_rc_data=0;
       high_getvalue++; //low_getvalue ++; 
       receive_first_bit_flag =0;
       

       if(low_getvalue < 10){ //get numbers 0; 

           if(receive_byte_flag==0){
               //get_bits(0x0,rf_data[0],receive_numbers);
               switch(receive_numbers){
                    case 0 : 
					{   
                        
                        rf_rec_data=0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 1 : 
					{ 
						rf_rec_data=0x40;  //
						break; 
					}
					case 2 : 
					{ 
						rf_rec_data=0x20; 
						break; 
					}//遥控编码第2位
					case 3 : 
					{ 
						rf_rec_data=0x10; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 4 :
					 { 
						rf_rec_data=0x08; 
						break; 
					}//遥控编码第4位
					case 5 : 
					{ 
						rf_rec_data=0x04; 
						break; 
					}//遥控编码第5位
					case 6 : 
					{ 
						rf_rec_data=0x02;
						break; 
					}//遥控编码第6位
					case 7 : 
					{
					   rf_rec_data=0x01; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
               receive_numbers ++;
               if(receive_numbers ==8){
                receive_numbers =0;
                receive_byte_flag=1;
                }
               low_getvalue=0;// high_getvalue=0;
           }
           else if(receive_byte_flag==1){

                switch(receive_numbers){
                    case 0 : 
					{   
                        
                        rf_rec_data1=0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 1 : 
					{ 
						rf_rec_data1=0x40;  //
						break; 
					}
					case 2 : 
					{ 
						rf_rec_data1=0x20; 
						break; 
					}//遥控编码第2位
					case 3 : 
					{ 
						rf_rec_data1=0x10; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 4 :
					 { 
						rf_rec_data1=0x08; 
						break; 
					}//遥控编码第4位
					case 5 : 
					{ 
						rf_rec_data1=0x04; 
						break; 
					}//遥控编码第5位
					case 6 : 
					{ 
						rf_rec_data1=0x02;
						break; 
					}//遥控编码第6位
					case 7 : 
					{
					   rf_rec_data1=0x01; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
              
               low_getvalue=0;//receive_numbers ++;
              if(receive_numbers ==8){
                receive_numbers =0;
                receive_byte_flag=2;
               }
              high_getvalue=0;
           }
           else if(receive_byte_flag==2){
                switch(receive_numbers){
                    case 0 : 
					{   
                        
                        rf_rec_data2=0x80; //最高位 
						break;
					 }   //遥控编码第1位
					case 1 : 
					{ 
						rf_rec_data2=0x40;  //
						break; 
					}
					case 2 : 
					{ 
						rf_rec_data2=0x20; 
						break; 
					}//遥控编码第2位
					case 3 : 
					{ 
						rf_rec_data2=0x10; 
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 4 :
					 { 
						rf_rec_data2=0x08; 
						break; 
					}//遥控编码第4位
					case 5 : 
					{ 
						rf_rec_data2=0x04; 
						break; 
					}//遥控编码第5位
					case 6 : 
					{ 
						rf_rec_data2=0x02;
						break; 
					}//遥控编码第6位
					case 7 : 
					{
					   rf_rec_data2=0x01; 
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
               low_getvalue=0;//receive_numbers ++;
               if(receive_numbers ==8){
                receive_numbers =0;
                receive_byte_flag=0;
                if(gpro_t.receive_data_success >2){
                    gpro_t.stop_receive_data = 1;
                    gpro_t.gTime_rf_rc_data=0;
                  }
                sync_start_flag=0;
                gpro_t.g_sync_flag=0;
                
               }
               low_getvalue=0;//high_getvalue=0;
           }
          
        }
        else{ //get number "1"
           if(receive_byte_flag==0){
              
                receive_numbers ++; 
                if(receive_numbers ==8){
                receive_numbers =0;
                receive_byte_flag=1;
                }
                low_getvalue=0;// high_getvalue=0;
           }
           else if(receive_byte_flag==1){
               
                 receive_numbers ++; 
                  if(receive_numbers ==8){
                   receive_numbers =0;
                   receive_byte_flag=2;
                   }
                 low_getvalue=0;//  high_getvalue=0;
           }
           else if(receive_byte_flag==2){
              
               receive_numbers ++; 
               if(receive_numbers ==8){
                receive_numbers =0;
                receive_byte_flag=0;
                gpro_t.receive_data_success ++ ;
                if(gpro_t.receive_data_success > 2){
                    gpro_t.stop_receive_data = 1;
                    gpro_t.gTime_rf_rc_data=0;

                    }
                sync_start_flag=0;
                gpro_t.g_sync_flag=0;
                
               }
               low_getvalue=0;//high_getvalue=0;
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


