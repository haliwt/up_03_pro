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
uint8_t up_flag , stop_receive_data_flag ;
uint8_t syn_flag;

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
        
        if(RF_KEY_GetValue()==1)      /* 上升沿捕获 */
        {


            up_dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);        /* 读取CCR1也可以清CC1IF标志位 *//* 标记下降沿已经被捕获 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);    /* 配置TIM3通道1下降沿捕获 */
          
            g_remote_sta |= 0X10;   
           
             __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器值 */
             if (up_dval > 9000  && up_dval < 10000 && up_flag > 0 && syn_flag  < 2) /*  315MHZ-低电平持续时间   9.76ms  *//* 4500为标准值4.5ms, */
             {
                    g_remote_sta |= 1 << 7; /* 标记成功接收到了引导码 */
                    gpro_t.g_sync_flag =1;
                    g_remote_cnt = 0;       /* 清除按键次数计数器 */
                    syn_flag ++;
                   // gpro_t.recieve_numbers=0;
                    if(syn_flag==1)gpro_t.recieve_numbers=0;
                    else{
                      stop_receive_data_flag=1;

                   }
              }

        }
        else           /* 下降沿捕获 */
        {
            dval=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);                /* 读取CCR1也可以清CC1IF标志位 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);     /* 配置TIM3通道1上升沿捕获 */
            __HAL_TIM_SET_COUNTER(&htim3, 0);  /* 清空定时器值 */

            if (g_remote_sta & 0X10)        /* 完成一次低电平捕获 */
            {
                if((g_remote_sta & 0X80) && gpro_t.g_sync_flag ==1 && stop_receive_data_flag == 0)   /* 接收到同步信号，低电平持续时间   9.76ms */
                {

                    if (dval > 100 && dval < 400)       /* 低电平大于 340us，小于max= 930us，低电平*/
                    {
                        //g_remote_data >>= 1;                /* 右移一位 */
                        
                        g_remote_data <<= 1;
                        g_remote_data &= ~(0x000001);     /* 接收到0 */
                       gpro_t.recieve_numbers++;
                      //  if(gpro_t.recieve_numbers > 100)stop_receive_data_flag=1;
                    }
                    else if (dval > 300 && dval < 1200)    /* 低电平小于   max= 340us ，是高电平  */
                    {
                        g_remote_data  <<= 1;                /* 右移一位 */
                        g_remote_data |= 0x000001;        /* 接收到1 */
                        gpro_t.recieve_numbers++;
                       // if(gpro_t.recieve_numbers > 100)stop_receive_data_flag=1;
                    }
//                    else if (dval > 6000 && dval < 20000)    /* 得到第二个地址码的，引导低电平，重复的地址码 */
//                    {
//                        g_remote_cnt++;         /* 按键次数增加1次 */
//                        g_remote_sta &= 0XF0;   /* 清空计时器 */
//                        if(g_remote_cnt==1)gpro_t.g_sync_flag =0;
//                        gpro_t.rf_rec_data = gpro_t.recieve_numbers;
//                        gpro_t.recieve_numbers=0;
//                    }
                }
                else if (dval > 200  && dval < 400) /*  315MHZ-低电平持续时间   9.76ms  *//* 4500为标准值4.5ms, */
                {
                    up_flag ++;
                   
                }
            }

            g_remote_sta&=~(1<<4); //清空下降沿标志位
        }
    }
}


//void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
//void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
//
//{
//
//   if(GPIO_Pin == RF_KEY_Pin){
//
//       
//        if(gpro_t.g_sync_flag == 1){
//            
//
//          gpro_t.rf_key_interrupt_numbers++ ;
//  
//        }
//     
//
//
//   }
//
//
//
//
//}


/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function call back function
	*timer17 is  1s ,TIM3 is 0.1ms=100us
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  static uint16_t tm0;
  static uint8_t tm1;
  
  if(htim->Instance==TIM3){

    
  
     rf_irqhandler();

  }
  else if(htim->Instance==TIM17){
    
    tm0++;  //1ms
	if(tm0 > 0){ //5s 
      tm0=0;
      tm1++;

      gpro_t.gTime_rf_rc_data++;
       gpro_t.gTimer_run_process++;
       gpro_t.gTimer_stop_receive++;
      

      if(tm1 > 11){ //1 minute.
         tm1 =0;
        
         gpro_t.gTimer_works_total_times_minutes++ ;
        

      }

    }
    
	}
}






