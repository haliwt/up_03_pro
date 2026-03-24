#include "bsp.h"
#include "app_azure_rtos.h"
#include "stm32g0xx.h"


/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
/* ????????? */
#define STACK_SIZE_ONE  2048//3072//2048//1024//896//768
//#define STATC_SIZE_TWO  512//256

static TX_THREAD thread_msg_pro;
//static TX_THREAD thread_start;
/* 定义信号量 */
//TX_SEMAPHORE remote_semaphore;


static UCHAR stack_msg_pro[STACK_SIZE_ONE];
//static UCHAR stack_start[STATC_SIZE_TWO];



static void vTaskMsgPro(ULONG thread_input);
//static void vTaskStart(ULONG thread_input);

//TX_THREAD remote_task;
//UCHAR remote_task_stack[STACK_SIZE];



uint8_t dc_power_on_first;


/**********************************************************************************************************
*
*	Function Name:
*	Function:
*	Input Ref:
*	Return Ref:
*
**********************************************************************************************************/


/**********************************************************************************************************
*	Function Name: vTaskMsgPro
*	Function:
*	Input Ref: *pvParameters
*	Return Ref:NO
*   
**********************************************************************************************************/
static void vTaskMsgPro(ULONG thread_input)
{
    (void)thread_input;  /* 消除未使用的参数警告 */
	
    while(1)
    {
		
      if(gpro_t.dc_power_on_first==0){
         gpro_t.dc_power_on_first++;
	      gpro_t.power_on = power_off;//WT.EDIT 2025.05.10
              
          led_on_fun();

     }

	  gpro_t.power_key_flag ++;
	 if(gpro_t.rf_complete_receive_flag ==1 && gpro_t.gTimer_rf_receive_counter > 3 ){//5 // 60ms *10 = 600ms = 0.6s

        gpro_t.rf_receive_data_success=0;
        gpro_t.rf_complete_receive_flag = 0;
        gpro_t.rf_rx_data_num=0;
        gpro_t.gTimer_rf_receive_counter=0;
         g_remote_data=0;
         rf_sync_signal_flag = 0;
       }
	   else if(KEY_POWER_GetValue()  == KEY_DOWN){
			gpro_t.power_key_flag ++;

	        tx_thread_sleep(100);
			
	      if(KEY_POWER_GetValue()  == KEY_DOWN){
			
		    gpro_t.rfPowerOnOff_soundFLag =0;
	        if(gpro_t.power_on == power_off){
                   gpro_t.power_on = power_on;
				   led_on_fun();
                   voice_power_on_sound();
				   fan_output_fun();

			    
         
			}
			else if(gpro_t.power_on == power_on){
			   gpro_t.power_on = power_off;
			   
			    led_off_fun();
			   voice_power_off_sound();
			   fan_stop_fun();
			    
			   

			}
	      }	   
	  }
      else if(gpro_t.power_on == power_on){

      
		if(gpro_t.fan_warning_flag ==0){
		      led_on_fun(); //WT.EDIT 2025.05.14
		     // fan_output_fun();
        }

         main_board_ctl_handler(gpro_t.works_2_hours_timeout_flag);
         device_works_time_counter_handler();

      	}
        else if(gpro_t.power_on == power_off){
          
              power_off_handler();
              led_off_fun();
        }

      
	
	 sound_power_on_off_handler();		  


     tx_thread_sleep(20);//2*10
             
    }
      
 }
/**********************************************************************************************************
 * 
*	Funtion Name:
*	Funtion:
*	Input Ref: pvParameters 
*	Return Ref:
*  
**********************************************************************************************************/
#if 0
static void vTaskStart(ULONG thread_input)
{
    (void)thread_input;  /* 消除未使用的参数警告 */
   while(1)
    {
	
		//bsp_KeyScan();
    if(KEY_POWER_GetValue()  == KEY_DOWN){

        gpro_t.power_key_flag = 1;
		/* 检测到按键按下，通知处理任务 */
        //tx_semaphore_put(&key_semaphore);
	
    }
     
    tx_thread_sleep(20);//3*10
  }
}
#endif 
/**********************************************************************************************************
* 
* Function Name: 
* Function: 
* Input Ref: NO
* Return Ref: NO
* 
**********************************************************************************************************/
/**
  * @brief  Define the initial system.
  * @param  first_unused_memory : Pointer to the first unused memory
  * @retval None
  */

void AppTaskCreate (void)
{
 tx_thread_create(&thread_msg_pro, "MsgPro",
                     vTaskMsgPro, 0,
                     stack_msg_pro, 
                     STACK_SIZE_ONE,
                     0,
                     0,
                     TX_NO_TIME_SLICE, 
                     TX_AUTO_START);
 #if 0

    tx_thread_create(&thread_start, "Start",
                     vTaskStart, 0,
                     stack_start, 
                     STATC_SIZE_TWO,
                     2, 
                     2, 
                     TX_NO_TIME_SLICE, 
                     TX_AUTO_START);
  #endif 

   /* 创建信号量 */
  // tx_semaphore_create(&remote_semaphore, "RemoteSemaphore", 0);
  
}


