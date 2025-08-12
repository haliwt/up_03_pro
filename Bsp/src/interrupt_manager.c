#include "bsp.h"

#define SYNC_MIN_US   6000    // 同步信号最小时间(us)
#define SYNC_MAX_US   9000     // 同步信号最大时间(us)

#define BIT0_MIN_US   180     // 0位最大时间(us)
#define BIT0_MAX_US   350     //  0位最大时间(us)

#define BIT1_MIN_US   400     // 1位最小时间(us)
#define BIT1_MAX_US   700     // 1位最大时间(us)

#define BITS_IN_PACKET 24    // 数据位数

/* 遥控器接收状�????
 * [7]  : 收到了引导码标志
 * [6]  : 得到了一个按键的�????有信�????
 * [5]  : 保留
 * [4]  : 标记下降沿是否已经被捕获
 * [3:0]: 溢出计时�????
 */
//uint8_t g_remote_state = 0;
//uint32_t g_remote_data = 0; /* 红外接收到的数据 */
//uint8_t  g_remote_cnt = 0;  /* 按键按下的次�???? */

//uint16_t dval,up_dval;  /* 下降沿时计数器的�???? */

//uint8_t  rf_syn_flag ;

// 全局变量
volatile uint32_t up_dval = 0;
volatile uint32_t dval = 0;
volatile uint8_t rf_syn_flag = 0;
volatile uint32_t g_remote_data = 0;
volatile uint8_t rf_receive_complete = 0;
volatile uint8_t bit_count = 0;

//volatile uint8_t detected_rfSync_flag = 0; /* �????测到遥控器代码标志位 */

 void RF_ResetDecoder(void)// 				// 丢帧，重新
 {

      if(RF_KEY_CMT2210LC_GetValue() == 1) {  // 上升沿捕获 (当前是低电平)
            up_dval = LL_TIM_IC_GetCaptureCH1(TIM3);
            LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);
            LL_TIM_SetCounter(TIM3, 0);
            
        // 同步信号检测
        if(up_dval > SYNC_MIN_US && up_dval < SYNC_MAX_US  && !gpro_t.rf_receive_data_success && rf_syn_flag==0) {
                rf_syn_flag = 1;
               // bit_count = 0;
            
        }
      }
      else {  // 下降沿捕获 (当前是高电平)
            dval = LL_TIM_IC_GetCaptureCH1(TIM3);
            LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
            LL_TIM_SetCounter(TIM3, 0);
            
            if(rf_syn_flag && !gpro_t.rf_receive_data_success) {
                // 数据位解码
                if(dval > BIT0_MIN_US && dval < BIT0_MAX_US) {        // 0位
                    g_remote_data = (g_remote_data << 1);
                    //bit_count++;
					gpro_t.rf_recieve_numbers++;
                } 
                else if(dval > BIT1_MIN_US && dval < BIT1_MAX_US) { // 1位
                    g_remote_data = (g_remote_data << 1) | 0x01;
                   // bit_count++;
					gpro_t.rf_recieve_numbers++;
                }
                
                // 检查是否接收完整数据包
                if(gpro_t.rf_recieve_numbers >= BITS_IN_PACKET) {
                 
                    rf_syn_flag = 0;
                    bit_count=gpro_t.rf_recieve_numbers;
                   
                    gpro_t.rf_recieve_numbers=0;
                    gpro_t.rf_receive_data_success=1;
                    gpro_t.rf_complete_receive_flag = 1;
                }
            }
        }
}



#if 0

/**
 * @brief       定时器输入捕获中断回调函�????
 * @param       htim:定时器句�????
 * @retval      �????
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == TIM3)
    {
      //  uint16_t dval;  /* 下降沿时计数器的�???? */
        
        if(RF_KEY_CMT2210LC_GetValue()==1)      /* 上升沿捕 rising capture signal->捕获的是低电平信,singnal is low level */
        {
            up_dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);        /* 读取CCR1也可以清CC1IF标志�???? *//* 标记下降沿已经被捕获 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);    /* 配置TIM3通道1下降沿捕�???? */
          
            __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器�?? */

             
             //synchronization signal times  BIG 6.5ms -6500us
             if (up_dval > 6300  && up_dval < 15000  &&  gpro_t.rf_receive_data_success ==0)//if (up_dval > 9000  && up_dval < 10000 && up_flag > 0 && syn_flag  < 2) /*  315MHZ-低电平持续时�????   9.76ms  *//* 4500为标准�??4.5ms, */
             {
                      rf_syn_flag = 1;//detected_rfSync_flag=1;
                  
					            gpro_t.rf_syn_signal_numbers++;
             
             }
             else if (rf_syn_flag==1){ /* 低电平持续时�???? 624us is bit "0" */
                   // g_remote_state |= 1 << 7; /* 标记成功接收到了引导�???? ,synchronizing signal*/
                   // g_remote_cnt = 0;       /* 清除按键次数计数�???? */
                    
                   
                    
                   
                  
            }
            

        }
        else    /* 下降沿捕�????   ，捕获的是高电平信号*/
        {
           
          dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);                /* 读取CCR1也可以清CC1IF标志�???? */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);     /* 配置TIM3通道1上升沿捕�???? */
            __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器�?? */

             // if(g_remote_state & 0X10)      /* 完成�????次低电平捕获 */
              {
                if(rf_syn_flag == 1 && gpro_t.rf_receive_data_success ==0)   /* 接收到同步信号，低电平持续时�????   9.76ms */
                {

                    if (dval > 150 && dval < 350)      /* high level more 212us，小于max= 350us，receive is bit "0"*/
                    {
                        //g_remote_data >>= 1;                /* 左移�????�???? */
                        
                        //g_remote_data <<= 1;              /* 左移�????�???? */
                        //g_remote_data &= ~(0x000001);     /* 接收�????0 */
                      g_remote_data = (g_remote_data << 1) | 0x0;
                      gpro_t.rf_recieve_numbers++;
                      
                      
                    }
                    else if (dval > 550  && dval < 1100)    /* 低电平小�????   max= 624us �?? biggest 900us receive is bit "1"  */
                    {
                        //g_remote_data  <<= 1;             /* 左移�????�???? */
                        //g_remote_data |= 0x000001;        /* 接收�????1 */
                        g_remote_data = (g_remote_data << 1) | 0x01;
                        gpro_t.rf_recieve_numbers++;
                       
                    }
                    if( gpro_t.rf_recieve_numbers > 23){
                          
                         gpro_t.rf_receive_data_success=1;
                         rf_syn_flag = 0;//detected_rfSync_flag=1;
                         gpro_t.rf_complete_receive_flag = 1; /* 接收完成标志�? */
					      gpro_t.rf_syn_signal_numbers=0;
                         
                    }

                }
             
              
            }

          //  g_remote_state&=~(1<<4); //清空下降沿标志位
            
        }
    }
}

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function call back function
	*timer 1ms 
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  static uint16_t tm0;
  static uint8_t tm1;
  
  if (htim->Instance == TIM16)
  {
    HAL_IncTick();
  }
  else if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	if(tm0 > 99){ //10ms *100 =100;
      tm0=0;
      tm1++;

    //  gpro_t.gTimer_adc_detected_time++;
   
      gpro_t.gTimer_normal_run_main_function_times++;
   //   gpro_t.gTimer_timer_time_long_key ++;
      gpro_t.gTimer_power_on_times++;
       gpro_t.gTimer_switch_onoff++;

      if(tm1 > 59){ //1 minute.
         tm1 =0;
    
         gpro_t.gTimer_works_total_times_minutes++ ;
        

      }

    }
    
	}
}


#endif 




