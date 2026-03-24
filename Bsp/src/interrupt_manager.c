#include "bsp.h"

#define SYNC_MIN_US   5500    // 同步信号�??小时�??(us)
#define SYNC_MAX_US   10000     // 同步信号�??大时�??(us)

#define BIT0_MIN_US   100     // 0位最大时�??(us)
#define BIT0_MAX_US   400//350     //  0位最大时�??(us)

#define BIT1_MIN_US   400//500     // 1位最小时�??(us)
#define BIT1_MAX_US   900//800     // 1位最大时�??(us)

#define BITS_IN_PACKET 25    // 数据位数

/* 遥控器接收状�??????
 * [7]  : 收到了引导码标志
 * [6]  : 得到了一个按键的�??????有信�??????
 * [5]  : 保留
 * [4]  : 标记下降沿是否已经被捕获
 * [3:0]: 溢出计时�??????
 */



// 全局变量
volatile uint32_t up_dval = 0;
volatile uint32_t dval = 0;
volatile uint8_t rf_sync_signal_flag = 0;
volatile uint32_t g_remote_data = 0;
uint32_t bit_num;


static void rf_recieve_data_fun(void);
/************************************************************************************************
*
*Function Name:void RF_ISR_receiveData(void)
*Function: 
*Input Ref:
*Return Ref:
*
**************************************************************************************************/
 void RF_ISR_receiveData(void)
 {
   
      if(GPIOA->IDR & GPIO_IDR_ID6) {  // 上升沿捕�?? (当前是低电平)
            up_dval = TIM3->CCR1;//up_dval = LL_TIM_IC_GetCaptureCH1(TIM3);
            TIM3->CCER =(TIM3->CCER & ~TIM_CCER_CC1NP)|TIM_CCER_CC1P;//LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);
            TIM3->CNT = 0; // 清零计数�?//LL_TIM_SetCounter(TIM3, 0);
            
        // 同步信号�??�??
      if(up_dval > SYNC_MIN_US && up_dval < SYNC_MAX_US && rf_sync_signal_flag==0){
                rf_sync_signal_flag = 1;
         }
      }
      else{  // 
          
            dval = TIM3->CCR1 ;//dval = LL_TIM_IC_GetCaptureCH1(TIM3);
            TIM3->CCER &=~(TIM_CCER_CC1P | TIM_CCER_CC1NP);//LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
            TIM3->CNT = 0; // CCER"capture/compare enable register"//LL_TIM_SetCounter(TIM3, 0);
            
            if(rf_sync_signal_flag && !gpro_t.rf_receive_data_success) {
                // 数据位解�??
                if(dval > BIT0_MIN_US && dval < BIT0_MAX_US) {        // 0�??
                    g_remote_data = (g_remote_data << 1);
                  
					   gpro_t.rf_rx_data_num++;
                    if(gpro_t.rf_rx_data_num >= BITS_IN_PACKET){
                       gpro_t.rf_receive_data_success=1;
                        rf_recieve_data_fun();
					             
				      }
                } 
                else if(dval > BIT1_MIN_US && dval < BIT1_MAX_US) { // 1�??
                    g_remote_data = (g_remote_data << 1) | 0x01;
                  
					 gpro_t.rf_rx_data_num++;
                    if(gpro_t.rf_rx_data_num >= BITS_IN_PACKET){
                       gpro_t.rf_receive_data_success=1;
                       rf_recieve_data_fun();
                       
			        }
                }
                

            }
        }
       
}
/************************************************************************************************
*
*Function Name:void bsp_init(void)
*Function: 1s this timer 
*Input Ref:
*Return Ref:
*
**************************************************************************************************/
void tim17_isr_callback(void)
 {
   
	static uint8_t tm1;
     tm1++;
     gpro_t.gTimer_normal_run_main_function_times++;
     gpro_t.gTimer_led_blink_counter++;
      if(tm1 > 59){ //1 minute.
         tm1 =0;
    
         gpro_t.gTimer_works_total_times_minutes++ ;
        
       }

}


    
/*
  * @ brief :
  * @ notice:
  * param:
*/
static void rf_recieve_data_fun(void)
{


  switch(pair_remote_ID_success_flag){


  case 1:


   bit_num =  g_remote_data & 0xFFFFFF;
   g_remote_data =0; //WT.EDIT 2025.10.21
   if(gpro_t.rf_decod_ID== bit_num  &&  bit_num !=0){ ////WT.EDIT 2025.10.21
    gpro_t.rf_complete_receive_flag = 1;
    gpro_t.gTimer_rf_receive_counter=0;
    gpro_t.rfPowerOnOff_soundFLag=1;
	rf_sync_signal_flag = 0; //WT.EDIT 2025.12.11
	gpro_t.rf_rx_data_num=0;//WT.EDIT 2025.12.11
    gpro_t.rf_complete_receive_flag = 0;//WT.EDIT 2025.12.11
	g_remote_data=0;//WT.EDIT 2025.12.11
   }          
   else {
      gpro_t.rf_receive_data_success=0;
      gpro_t.rf_complete_receive_flag = 0;
      gpro_t.rf_rx_data_num=0;
      rf_sync_signal_flag = 0;
      gpro_t.rfPowerOnOff_soundFLag=0; 
	  g_remote_data=0;//WT.EDIT 2025.12.11

    }

 
  break;

  case 0:
 
      gpro_t.rf_receive_data_success=0;
      gpro_t.rf_complete_receive_flag = 0;
      gpro_t.rf_rx_data_num=0;
      rf_sync_signal_flag = 0;
      remoteControl_ID_firstPair433Mhz_handler();
  break;

  	}
}

  






