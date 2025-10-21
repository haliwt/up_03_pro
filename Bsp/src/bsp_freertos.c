#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"



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
//static void vTaskLedPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
//static void AppObjCreate (void);
//static void vTimer1Callback(xTimerHandle pxTimer);
//static void vTimer2Callback(xTimerHandle pxTimer);



/***********************************************************************************************************
											变量声明
***********************************************************************************************************/

//static TaskHandle_t xHandleTaskLedPro = NULL;

static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
//static TimerHandle_t xTimers[2] = {NULL};

//static TimerHandle_t           Timer1Timer_Handler;/* 定时�?????1�?????71句柄 */
//static TimerHandle_t           Timer2Timer_Handler;/* 定时�?????1�?????72句柄 */


uint8_t dc_power_on_first;


/**********************************************************************************************************
*
*	Function Name:
*	Function:
*	Input Ref:
*	Return Ref:
*
**********************************************************************************************************/
void freertos_handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
	//AppObjCreate();
	
    /* 启动调度，开始执行任�?????1�?????7 */
    vTaskStartScheduler();
}

/**********************************************************************************************************
*	Function Name: vTaskMsgPro
*	Function:
*	Input Ref: *pvParameters
*	Return Ref:NO
*   
**********************************************************************************************************/
static void vTaskMsgPro(void *pvParameters)
{

    while(1)
    {
		
      if(dc_power_on_first==0){
        dc_power_on_first++;
			   gpro_t.power_on = power_off;//WT.EDIT 2025.05.10
              
          led_on_fun();
          osDelay(400);
			   led_off_fun();
			   osDelay(400);
			   led_on_fun();
			   osDelay(400);
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
        gpro_t.rf_recieve_numbers=0;
        gpro_t.gTimer_rf_receive_counter=0;
         g_remote_data=0;
         rf_sync_signal_flag = 0;
        }
	


    vTaskDelay(10);
             
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
static void vTaskStart(void *pvParameters)
{
   
   while(1)
    {
	
		//bsp_KeyScan();
    if(KEY_POWER_GetValue()  == KEY_DOWN){

        gpro_t.power_key_flag = 1;
	
    }
     
    vTaskDelay(20);
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
static void AppTaskCreate (void)
{

  xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务�?????1�?????7    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* priority is 1*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务�?????1�?????7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* priority is 2*/
                 &xHandleTaskStart );   /* 任务句柄  */
}


