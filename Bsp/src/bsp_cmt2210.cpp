#include "bsp.h"


static void sync_single(void);


uint8_t sync_start_flag;
uint8_t decode_ok;            //解码成功
uint16_t  high_getvalue,low_getvalue;           //高,低电平宽度
uint8_t  receive_decoder_numbers;//ma_x;                //接收到第几位编码了
uint8_t  bma1,bma2,bma3,bma4; //用于接收过程存放遥控编码，编码比较两次，这是第一次
uint8_t  mma1,mma2,mma3,mma4;
uint8_t  mmb1,mmb2,mmb3,mmb4; // 用于接收过程存放遥控编码，第二次

uint8_t rf_ok1,rf_ok2,rf_ok;         //解码过程中的临时接收成功标志,接收到一个完整的遥控命令后置1,通知解码程序可以解码了
uint8_t low_level_rc;//uint8_t old_rc5;             //保存上一次查询到的电平状态
uint8_t sync_flag; //tb_ok;               //接收到同步的马时置1   
uint8_t D0,D1,D2,D3 ;
uint16_t s ,s1; 
uint8_t bt_auto;     //自动设置遥控接收波特率标志
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
  
   if(RF_KEY_GetValue()==0   && low_start_flag ==0 && sync_start_flag ==0){
         
          high_rc=0;
         low_rc++;
         if(low_rc > 200) low_rc = 0; //200*10us = 500us


    }
    else if(RF_KEY_GetValue()==1 && (  low_rc < 90 && low_rc >30) && sync_start_flag ==0 &&  high_end_flag ==0){
         low_start_flag =1;
        

         high_rc++;

         if(high_rc > 50)high_rc=0;
           
           
    }
    else if(RF_KEY_GetValue()==1 && low_start_flag==0){
       low_rc=0;
    }


    

    if(RF_KEY_GetValue()==0 && (high_rc <  90 && high_rc > 30)  && sync_start_flag ==0 && low_start_flag ==1){
           high_end_flag= 1;
          low_rc++;
     

          if(low_rc > 1500) low_rc =0; //1500x 10us = 15000us =15ms

     }
     else if(RF_KEY_GetValue()==1 &&   (low_rc <= 1500 && low_rc >700) && sync_start_flag ==0 &&  high_end_flag== 1){
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
      else if(RF_KEY_GetValue()==1 &&   (low_rc > 1500 || low_rc  < 700) && sync_start_flag ==0 &&  high_end_flag== 1){

         low_rc =0;


      }

     
     if(RF_KEY_GetValue()==1   && sync_start_flag ==1 && high_end_flag==2){

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

   if(RF_KEY_GetValue()==0 &&  sync_start_flag ==1){ //

        low_getvalue ++; 
        low_level_rc = 0; 
        high_rc=0;

   }
   else if(sync_start_flag ==1){
    
      high_getvalue++;
      if(!low_level_rc){// 检测到从低到高的跳变,已检测		 // 到一个完整(高-低)电平周期
//         if(low_getvalue >= 100 && low_getvalue <=140){ //同步电平，低电平10ms,100*100us=10000us
//            sync_flag =1;
//            receive_decoder_numbers= 0;
//            bma1=0;
//            bma2=0;
//            bma3=0;
//            bma4=0;
//         }
         //else 
         if((sync_flag == 1) &&low_getvalue>=10 && low_getvalue<=15){ //500*10us=5ms,接收完成一次信号

             if(receive_decoder_numbers > 23){ //the 24 byte is key value

                 if(rf_ok1==0){
                  mma1= bma1;
                  mma2 = bma2;
                  mma3= bma3;
                  mma4 = bma4;
                  rf_ok1 =1;
                  sync_flag = 0;
                  s=1000;
                  }
                  else{ //将接收到的编码复制到解码寄存器中  
                     mmb1 = bma1;
                     mmb2 = bma2;
                     mmb3 = bma3;
                     mmb4 = bma4;
                     rf_ok2= 1;
                     sync_flag =0;
                     sync_start_flag =0;


                  }
                }
                receive_decoder_numbers++;
             }
             else if((sync_flag ==1)&& low_getvalue>=30 && low_getvalue <=150){ //

                switch (receive_decoder_numbers)   //地址码，20位。20bit
				{ 
							case 0 : 
							{ 
								bma1=bma1 | 0x80; //最高位 
								break;
							 }   //遥控编码第1位
							case 1 : 
							{ 
								bma1=bma1 | 0x40;  //
								break; 
							}
							case 2 : 
							{ 
								bma1=bma1 | 0x20; 
								break; 
							}//遥控编码第2位
							case 3 : 
							{ 
								bma1=bma1 | 0x10; 
								break; 
							}//遥控编码第3位 --> 1个字节 高4位
							case 4 :
							 { 
								bma1=bma1 | 0x08; 
								break; 
							}//遥控编码第4位
							case 5 : 
							{ 
								bma1=bma1 | 0x04; 
								break; 
							}//遥控编码第5位
							case 6 : 
							{ 
								bma1=bma1 | 0x02;
								break; 
							}//遥控编码第6位
							case 7 : 
							{
							   bma1=bma1 | 0x01; 
								break;
							 }//遥控编码第7位  ----> 1个字节 低4位
							
							case 8 :
							 { 
								bma2=bma2 | 0x80;  //下一个 2 byte 
								break; 
							}//遥控编码第9位
							case 9 : 
							{
								bma2=bma2 | 0x40; 
								break; 
							}//遥控编码第10位
							case 10:
							 { 
								bma2=bma2 | 0x20; 
								break;
							 }
							case 11: 
							{ 
								bma2=bma2 | 0x10; 
								break; 
							}//遥控编码第10位
							case 12: 
							{ 
								bma2=bma2 | 0x08; 
								break;
						    }
							case 13:
							 { 
								bma2=bma2 | 0x04;
								break; 
							}
							case 14: { bma2=bma2 | 0x02; 
													break; }
							case 15: 
							{ 
								bma2=bma2 | 0x01; //第三字节， 3 byte
								break; 
							}
							
							case 16: 
							{ 
								bma3=bma3 | 0x80; 
								break; 
							}
							case 17: { bma3=bma3 | 0x40; 
													break; }
							case 18: { bma3=bma3 | 0x20; 
													break; }
							case 19: 
							{  
								bma3=bma3 | 0x10; //高字节  4 BIT 
								break; 
						    }
							case 20: //315MHZ 数据位
							 {
								bma3=bma3 | 0x08; 
								break; 
							}// 按键状态第1位
							case 21: 
							{ 
								bma3=bma3 | 0x04; 
								break; 
						    }
							case 22: 
							{ 
								bma3=bma3 | 0x02; 
								break; 
							}
							case 23: 
							{ 
									bma3=bma3 | 0x01; 

								if(!rf_ok1)
								{
									mma1=bma1;
									mma2=bma2;
									mma3=bma3; // 将接收到的编码复制到解码寄存器中             		 
									rf_ok1=1;         // 通知解码子程序可以解码了
									s=1000;                                
								}
								else
								{
									mmb1=bma1;
									mmb2=bma2;
									mmb3=bma3; // 将再次接收到的编码复制到解码寄存器中，                             
									rf_ok2=1;  // 通知解码子程序可以解码了	                                                                         
								} 
								sync_flag=0;
								break; 		
                             }
                    }
                    receive_decoder_numbers ++;

             }
             else{
                  //接收到不符合的高-低电平序列
					receive_decoder_numbers=0; 
					sync_flag=0;
					bt_auto=0;
					bma1=0;
					bma2=0; 
					bma3=0; 
					high_getvalue=0;
				    low_getvalue=0;
				}              
            high_getvalue=0;
            low_getvalue=0;


             }
            low_level_rc =1; //recoder itself how level state.


         }//high lveel end
        

    if(rf_ok1 && sync_start_flag ==1){ //规定时间内接收到2帧相同的编码数据，才有效。

       s--;
       if(!s){

          rf_ok1=0;

       }

       if(rf_ok2 && sync_start_flag ==1){

           if((mma1 == mmb1)&&(mma2==mmb2) &&(mma3==mmb3)){

                 rf_ok =1; 
           }
           else{

              rf_ok1=0;
              rf_ok2= 0;
           }

       }

    }


    if(rf_ok && sync_start_flag ==1){  //判断是否接收成功

        rf_ok =0;
        rf_data[0]=mma1;
        rf_data[1]=mma2;
        rf_data[2]=mma3;

        decode_ok =1;
        sync_start_flag =0;

    }
     

 }




