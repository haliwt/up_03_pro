#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"



/***********************************************************************************************************
											宏定�????1�????7
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

//static TimerHandle_t           Timer1Timer_Handler;/* 定时�????1�????71句柄 */
//static TimerHandle_t           Timer2Timer_Handler;/* 定时�????1�????72句柄 */






uint8_t dc_power_on_first;



//typedef struct Msg
//{

// 
//    //uint8_t  rfPowerOnOff_soundFLag;
//    
// //   uint8_t  power_key_flag ;

//  //  uint8_t  power_on_off_numbers;
//    
//   
//}MSG_T;

//MSG_T   g_tmsg; /* 定义丢�个结构体用于消息队列 */



/**********************************************************************************************************
*	�????1�????7 �????1�????7 �????1�????7: main
*	功能说明: 标准c程序入口�????1�????7
*	�????1�????7    参：�????1�????7
*	�????1�????7 �????1�????7 �????1�????7: �????1�????7
**********************************************************************************************************/
void freertos_handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
	//AppObjCreate();
	
    /* 启动调度，开始执行任�????1�????7 */
    vTaskStartScheduler();
}

/**********************************************************************************************************
*	�????1�????7 �????1�????7 �????1�????7: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发��的事件标志位设�????1�????7
*	�????1�????7    �????1�????7: pvParameters 是在创建该任务时传��的形参
*	�????1�????7 �????1�????7 �????1�????7: �????1�????7
*   �????1�????7 �????1�????7 �????1�????7: 2  
**********************************************************************************************************/
static void vTaskMsgPro(void *pvParameters)
{
//    BaseType_t xResult;
//	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置朢�大等待时间为50ms */
//	uint32_t ulValue;


  //  static uint8_t switch_onoff;
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
              // VOICE_SOUND();
               
      }


         if(gpro_t.power_key_flag == 1 &&  KEY_POWER_GetValue()  == KEY_UP){

                gpro_t.power_key_flag ++;
               
                powerOnOff_handler();
                  
           }
            
                 
       sound_power_on_off_handler();

      if(gpro_t.power_on == power_on ){

    
		    led_on_fun(); //WT.EDIT 2025.05.14

         main_board_ctl_handler(gpro_t.works_2_hours_timeout_flag);
         device_works_time_counter_handler();

      	}
        else if(gpro_t.power_on == power_off){
             gpro_t.works_2_hours_timeout_flag=0;
             gpro_t.fan_warning_flag = 0;
              
              // voice_power_off_sound();

             
        
              power_off_handler();
              led_off_fun();
            
              
             
          }

    vTaskDelay(20);
             
    }
      
 }
/**********************************************************************************************************
*	�????1�????7 �????1�????7 �????1�????7: vTaskStart
*	功能说明: 启动任务，也就是朢�高优先级任务，这里用作按键扫描��????1�????7
*	�????1�????7    �????1�????7: pvParameters 是在创建该任务时传��的形参
*	�????1�????7 �????1�????7 �????1�????7: �????1�????7
*   �????1�????7 �????1�????7 �????1�????7: 3  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
   //BaseType_t xResult;
   ///const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置朢�大等待时间为500ms */

    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();
    if(KEY_POWER_GetValue()  == KEY_DOWN){

         vTaskDelay(pdMS_TO_TICKS(10));//osDelay(10); //WT.EDIT 2025.05.07
		 if(KEY_POWER_GetValue()  == KEY_DOWN){
             gpro_t.power_key_flag = 1;
		 }
    }
    else if(gpro_t.rf_complete_receive_flag  == 1 && dc_power_on_first==1){ //wirleless remote

              gpro_t.rf_complete_receive_flag ++;
              rfReceivedData_theFirst433MHZ_Handler();

     }
    vTaskDelay(20);
  }
}
/**********************************************************************************************************
*	�????1�????7 �????1�????7 �????1�????7: AppTaskCreate
*	功能说明: 创建应用任务
*	�????1�????7    参：�????1�????7
*	�????1�????7 �????1�????7 �????1�????7: �????1�????7
**********************************************************************************************************/
static void AppTaskCreate (void)
{

  xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务�????1�????7    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级次�????1�????7*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务�????1�????7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级最�????1�????7*/
                 &xHandleTaskStart );   /* 任务句柄  */
}

/*****************************************************************************
 * 
 * Function Name: 
 * Function:
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/


