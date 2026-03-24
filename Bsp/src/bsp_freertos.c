#include "bsp.h"
#include "app_azure_rtos.h"
#include "stm32g0xx.h"






/***********************************************************************************************************
											宏定�?????1�?????7
***********************************************************************************************************/
#define POWER_KEY_0	        (1 << 0)

#define TIMER_KEY_1         (1<< 1)

#define TIMER_LONG_KEY_2     (1<<2)

#define POWER_OFF_BIT_3    (1<< 3)

/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
/* ????????? */
#define STACK_SIZE_ONE  512
#define STATC_SIZE_TWO  256

static TX_THREAD thread_msg_pro;
static TX_THREAD thread_start;
/* 定义信号量 */
//TX_SEMAPHORE remote_semaphore;


static UCHAR stack_msg_pro[STACK_SIZE_ONE];
static UCHAR stack_start[STATC_SIZE_TWO];



static void vTaskMsgPro(ULONG thread_input);
static void vTaskStart(ULONG thread_input);

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
          tx_thread_sleep(40);
			   led_off_fun();
			   tx_thread_sleep(40);
			   led_on_fun();
			   tx_thread_sleep(40);
			   led_on_fun();
              
               
      }


         if(gpro_t.power_key_flag == 1 &&  KEY_POWER_GetValue()  == KEY_UP){

              gpro_t.power_key_flag ++;
              gpro_t.rfPowerOnOff_soundFLag =1;
              
		       
         }
            
        sound_power_on_off_handler();        
      

      if(gpro_t.power_on == power_on){

        if(gpro_t.fan_warning_flag ==0){
		      led_on_fun(); //WT.EDIT 2025.05.14
        }

         main_board_ctl_handler(gpro_t.works_2_hours_timeout_flag);
         device_works_time_counter_handler();

      	}
        else if(gpro_t.power_on == power_off){
          
              power_off_handler();
              led_off_fun();
        }

       if(gpro_t.rf_complete_receive_flag ==1 && gpro_t.gTimer_rf_receive_counter > 5 ){ // 60ms *10 = 600ms = 0.6s

        gpro_t.rf_receive_data_success=0;
        gpro_t.rf_complete_receive_flag = 0;
        gpro_t.rf_rx_data_num=0;
        gpro_t.gTimer_rf_receive_counter=0;
         g_remote_data=0;
         rf_sync_signal_flag = 0;
        }
	


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
static void vTaskStart(ULONG thread_input)
{
    (void)thread_input;  /* 消除未使用的参数警告 */
   while(1)
    {
	
		//bsp_KeyScan();
    if(KEY_POWER_GetValue()  == KEY_DOWN){

        gpro_t.power_key_flag = 1;
	
    }
     
    tx_thread_sleep(3);//3*10
  }
}
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
                     stack_msg_pro, STACK_SIZE_ONE,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_thread_create(&thread_start, "Start",
                     vTaskStart, 0,
                     stack_start, STATC_SIZE_TWO,
                     2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);


   /* 创建信号量 */
  // tx_semaphore_create(&remote_semaphore, "RemoteSemaphore", 0);
  
}


