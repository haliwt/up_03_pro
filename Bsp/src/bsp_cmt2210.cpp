#include "bsp.h"


static void sync_single(void);
static void get_bits(uint8_t value,uint8_t pbuf,uint8_t sbit);

static void rf_recieve_first_data(void);
static void rf_receive_second_data(void);
static void rf_receive_first_low_level_data(void);



uint8_t byte_switch_falg,receive_byte_flag;

uint16_t low_rc,high_rc,low_sync,start_rc_flag;

uint8_t low_rc_times,rec_low_times;

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
    
         rec_low_times++;
        // if(low_rc > 200) low_rc = 0; //200*10us = 500us 100usx 50
    }
    else if(RF_KEY_GetValue()==1 &&   (low_rc < 800 && low_rc >85) && gpro_t.g_sync_flag ==0){  //10ms 
            gpro_t.high_level_getvalue++;
            low_rc_times = rec_low_times;
         

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
   
         rec_low_times=0;


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

  
   if(RF_KEY_GetValue()==1){ //
       
       gpro_t.high_level_getvalue++;
      
       
       if((gpro_t.low_level_getvalue >85 && gpro_t.low_level_getvalue < 800)){

                gpro_t.recieve_numbers=0;
 
                 receive_byte_flag=0;
                gpro_t.second_low_level_adjsut=1;
                gpro_t.stop_receive_data ++;
              
                gpro_t.low_level_getvalue=0;
       }
       else 
           gpro_t.low_level_getvalue=0;
   
       if(rc_l_num != rc_l){
           rc_l_num = rc_l;
           rc_h++;
 
       }

   }
   else if(RF_KEY_GetValue()==0){
      
     
     gpro_t.low_level_getvalue ++; //gpro_t.high_level_getvalue++; 
     if(gpro_t.stop_receive_data < 2){
         gpro_t.g_sync_flag=1;

          if(rc_h_num != rc_h){
           rc_h_num = rc_h;
           rc_l++;
           gpro_t.recieve_numbers++;
 
          }


       if((gpro_t.high_level_getvalue >4 && gpro_t.high_level_getvalue < 800)&& gpro_t.stop_receive_data  !=2){ //display "1"

           if(gpro_t.stop_receive_data==0 ){
                rf_recieve_first_data();
             
            }
            else if(gpro_t.stop_receive_data==1){

                  rf_receive_second_data();
            }
          }
          else if((gpro_t.high_level_getvalue < 5  && gpro_t.high_level_getvalue   > 1) &&gpro_t.stop_receive_data  !=2){ //get number "0 "
         
           
              rf_receive_first_low_level_data();

            
          }
          else if(gpro_t.high_level_getvalue > 900 ){
              
                gpro_t.recieve_numbers=0;
 
                gpro_t.low_level_getvalue=0;
                gpro_t.high_level_getvalue=0;
                gpro_t.rf_rec_data2=0xff;
                gpro_t.rf_rec_data2_2 =0xff;
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
                         gpro_t.high_level_getvalue=0;
                            return ;
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data= 0x40;  //
						    gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data=0x20; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data= 0x10; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data=0x08; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data= 0x04; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data= 0x02;
                           gpro_t.high_level_getvalue=0;
                            return ;
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
               gpro_t.high_level_getvalue=0;// gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==1){

                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data1= 0x80; //最高位 
                          gpro_t.high_level_getvalue=0;
                            return ;
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						gpro_t.rf_rec_data1= 0x40;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						gpro_t.rf_rec_data1= 0x20;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						gpro_t.rf_rec_data1= 0x10;

                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						gpro_t.rf_rec_data1= 0x08;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data1= 0x04;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data1= 0x02;
                          gpro_t.high_level_getvalue=0;
                            return ;
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
                gpro_t.high_level_getvalue=0;//gpro_t.recieve_numbers ++;
           }
           else if(receive_byte_flag==2){
                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                       
                        gpro_t.rf_rec_data2 = 0x80; //最高位 
                          gpro_t.high_level_getvalue=0;
                            return ;
                        
                      
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data2 =0x40;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x20;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x10;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data2 = 0x08;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x04;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						 gpro_t.rf_rec_data2 = 0x02;
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					     gpro_t.rf_rec_data2 = 0x01; 
                           gpro_t.high_level_getvalue=0;
                          
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
                
        
               if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
               
              //  gpro_t.stop_receive_data++;
                
                gpro_t.gTime_rf_rc_data=0;
             
          
              
             
               }
                gpro_t.high_level_getvalue=0;//gpro_t.high_level_getvalue=0;
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
             gpro_t.high_level_getvalue=0;
         }
         else if(receive_byte_flag==1){
             
            if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=2;
                
            }
            gpro_t.high_level_getvalue=0;
         }
         else if(receive_byte_flag==2 &&  gpro_t.second_low_level_adjsut==0){
            
            
             if(gpro_t.recieve_numbers ==8){
              gpro_t.recieve_numbers =0;
              receive_byte_flag=0;
           
              //gpro_t.stop_receive_data++;
              }
             gpro_t.high_level_getvalue=0;
         
         
         
          }
          else if(receive_byte_flag==2 &&  gpro_t.second_low_level_adjsut==1){
            
            
             if(gpro_t.recieve_numbers ==8){
              gpro_t.recieve_numbers =0;
              receive_byte_flag=0;
           
              // gpro_t.stop_receive_data++;
               if(gpro_t.stop_receive_data > 0){
                    gpro_t.receive_data_success=1;
                    gpro_t.gTimer_stop_receive=0;
                  

                }  

             
              low_rc=0;
           
              }
              gpro_t.high_level_getvalue=0;//gpro_t.high_level_getvalue=0;
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
                          gpro_t.high_level_getvalue=0;
                            return ;
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						 gpro_t.rf_rec_data_2= 0x40;  //
						   gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						 gpro_t.rf_rec_data_2=0x20; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						 gpro_t.rf_rec_data_2= 0x10; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						 gpro_t.rf_rec_data_2= 0x08; 
                           gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data_2= 0x04; 

                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data_2= 0x02;
                          gpro_t.high_level_getvalue=0;
                            return ;
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
                gpro_t.high_level_getvalue=0;// gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==1){

                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                        
                        gpro_t.rf_rec_data1_2= 0x80; //最高位 

                          gpro_t.high_level_getvalue=0;
                            return ;
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x40;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x20;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x10;

                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						gpro_t.rf_rec_data1_2= 0x08;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x04;
                          gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						gpro_t.rf_rec_data1_2= 0x02;
                          gpro_t.high_level_getvalue=0;
                            return ;
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
               gpro_t.high_level_getvalue=0;
           }
           else if(receive_byte_flag==2){
                switch(gpro_t.recieve_numbers){
                    case 1 : 
					{   
                       
                        gpro_t.rf_rec_data2_2 = 0x80; //最高位 
                          gpro_t.high_level_getvalue=0;
                            return ;
                        
                      
						break;
					 }   //遥控编码第1位
					case 2 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x40;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}
					case 3 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x20;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第2位
					case 4 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x10;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第3位 --> 1个字节 高4位
					case 5 :
					 { 
						  gpro_t.rf_rec_data2_2 = 0x08;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第4位
					case 6 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x04;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第5位
					case 7 : 
					{ 
						  gpro_t.rf_rec_data2_2 = 0x02;
                            gpro_t.high_level_getvalue=0;
                            return ;
						break; 
					}//遥控编码第6位
					case 8 : 
					{
					      gpro_t.rf_rec_data2_2 = 0x01; 
                            gpro_t.high_level_getvalue=0;
                            
						break;
					 }//遥控编码第7位  ----> 1个字节 低4位
                }
                
             
              if(gpro_t.recieve_numbers ==8){
                gpro_t.recieve_numbers =0;
                receive_byte_flag=0;
               
              //  gpro_t.stop_receive_data++;
                if(gpro_t.stop_receive_data > 0){
                    gpro_t.receive_data_success=1;
                    gpro_t.gTimer_stop_receive=0;
                   

                }  
                gpro_t.gTime_rf_rc_data=0;

           
              
               }
                gpro_t.high_level_getvalue=0;
             
         
            }
}


/**
 * @brief       处理红外按键(类似按键扫描)
 * @param       无
 * @retval      0   , 没有任何按键按下
 *              其他, 按下的按键键值
 */
uint8_t remote_scan(void)
{
    uint8_t sta = 0;
    uint8_t t1, t2;

    if (g_remote_sta & (1 << 6))    /* 得到一个按键的所有信息了 */
    {
        t1 = g_remote_data;                 /* 得到地址码 */

        t2 = t1 & 0x08;
        if(t2==0x80){

           if(gpro_t.power_on == power_off){
              gpro_t.power_on = power_on;
              //cancel run 2 hours times 
             gpro_t.works_2_hours_timeout_flag =0;
             gpro_t.gTimer_works_total_times_minutes =0;
            

         }
         else{

            gpro_t.power_on = power_off;
            gpro_t.works_2_hours_timeout_flag =0;
            gpro_t.gTimer_works_total_times_minutes =0;

         }


        }
        //t2 = (g_remote_data >> 8) & 0xff;   /* 得到地址反码 */

//        if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)    /* 检验遥控识别码(ID)及地址 */
//        {
//            t1 = (g_remote_data >> 16) & 0xff;
//            t2 = (g_remote_data >> 24) & 0xff;
//
//            if (t1 == (uint8_t)~t2)
//            {
//                sta = t1;           /* 键值正确 */
//            }
//        }

        if ((sta == 0) || ((g_remote_sta & 0X80) == 0)) /* 按键数据错误/遥控已经没有按下了 */
        {
            g_remote_sta &= ~(1 << 6);  /* 清除接收到有效按键标识 */
            g_remote_cnt = 0;           /* 清除按键次数计数器 */
             gpro_t.g_sync_flag = 0;
        }
    }

    return sta;
}





