#include "bsp.h"


/* 遥控器接收状态
 * [7]  : 收到了引导码标志
 * [6]  : 得到了一个按键的所有信息
 * [5]  : 保留
 * [4]  : 标记下降沿是否已经被捕获
 * [3:0]: 溢出计时器
 */
uint8_t g_remote_sta = 0;
uint32_t g_remote_data = 0; /* 红外接收到的数据 */
uint8_t  g_remote_cnt = 0;  /* 按键按下的次数 */

uint16_t dval,up_dval;  /* 下降沿时计数器的值 */

uint8_t  rf_syn_flag ;



/**
 * @brief       定时器输入捕获中断回调函数
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
      //  uint16_t dval;  /* 下降沿时计数器的值 */
        
        if(RF_KEY_GetValue()==1)      /* 上升沿捕获    捕获的是低电平信号*/
        {
            up_dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);        /* 读取CCR1也可以清CC1IF标志位 *//* 标记下降沿已经被捕获 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);    /* 配置TIM3通道1下降沿捕获 */
          
            __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器值 */

             g_remote_sta |= 0X10; 
             if (up_dval > 8000  && up_dval < 12000  &&  gpro_t.rf_receive_data_success ==0 && gpro_t.rf_decoder ==0)//if (up_dval > 9000  && up_dval < 10000 && up_flag > 0 && syn_flag  < 2) /*  315MHZ-低电平持续时间   9.76ms  *//* 4500为标准值4.5ms, */
             {
                    g_remote_sta |= 1 << 7; /* 标记成功接收到了引导码 ,synchronizing signal*/
                   // g_remote_cnt = 0;       /* 清除按键次数计数器 */
                    rf_syn_flag = 1;
                    gpro_t.rf_syn_signal_numbers++;
                    
                    if(gpro_t.rf_syn_signal_numbers > 1 && gpro_t.rf_syn_signal_numbers < 7){

                          if(gpro_t.powerOn_matchingId !=10){

                               gpro_t.rf_syn_signal_numbers=0;
  
                              gpro_t.rf_receive_data_success=1;
                              gpro_t.rf_decoder =0;
                              rf_syn_flag = 0;


                          }
                          else{
                             gpro_t.rf_syn_signal_numbers=0;

                            gpro_t.rf_receive_data_success=1;
                            gpro_t.rf_decoder =1;
                            rf_syn_flag = 0;

                         }

                    }
                  
                      
             }
             else  if (up_dval > 8000  && up_dval < 12000  &&  gpro_t.rf_receive_data_success ==1){

                        g_remote_cnt++;


             }

        }
        else           /* 下降沿捕获   ，捕获的是高电平信号*/
        {
            dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);                /* 读取CCR1也可以清CC1IF标志位 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);     /* 配置TIM3通道1上升沿捕获 */
            __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器值 */

              if(g_remote_sta & 0X10)      /* 完成一次低电平捕获 */
              {
                if((g_remote_sta & 0X80) &&  rf_syn_flag == 1 && gpro_t.rf_receive_data_success ==0)   /* 接收到同步信号，低电平持续时间   9.76ms */
                {

                    if (dval > 200 && dval < 500)       /* high level more 340us，小于max= 930us，低电平，接收数据是 “0”*/
                    {
                        //g_remote_data >>= 1;                /* 左移一位 */
                        
                        //g_remote_data <<= 1;              /* 左移一位 */
                        //g_remote_data &= ~(0x000001);     /* 接收到0 */
                      g_remote_data = (g_remote_data << 1) | 0x0;
                      gpro_t.rf_recieve_numbers++;
                      
                    }
                    else if (dval > 700  && dval < 1100)    /* 低电平小于   max= 340us ，是高电平,接收到数据  “1”  */
                    {
                        //g_remote_data  <<= 1;             /* 左移一位 */
                        //g_remote_data |= 0x000001;        /* 接收到1 */
                        g_remote_data = (g_remote_data << 1) | 0x01;
                        gpro_t.rf_recieve_numbers++;
                       
                    }

                }
             
              
            }

            g_remote_sta&=~(1<<4); //清空下降沿标志位
            
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
  

  if(htim->Instance==TIM17){
    
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






