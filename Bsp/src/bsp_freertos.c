#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"



/***********************************************************************************************************
											宏定义
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

//static TimerHandle_t           Timer1Timer_Handler;/* 定时器1句柄 */
//static TimerHandle_t           Timer2Timer_Handler;/* 定时器2句柄 */





typedef struct Msg
{

    uint8_t  works_mode_flag;
    uint8_t  power_onoff_sound_flag;
    uint8_t  timer_sound_flag ;
    uint8_t  key_timer_timing_value;
    uint8_t  key_long_timer_flag;
  
    uint8_t  interval_flag;
    
   
}MSG_T;

MSG_T   g_tmsg; /* 定义一个结构体用于消息队列 */


/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freertos_handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
	//AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}

/**********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
**********************************************************************************************************/
static void vTaskMsgPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为50ms */
	uint32_t ulValue;
    static uint8_t dc_power_sound_flag, set_timer_hold_flag,voice_set_timer_flag;
    static uint8_t dc_power_on_first;
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/

 
		
	  xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间,等待时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0){

                 g_tmsg.power_onoff_sound_flag =1;
                
            }
            else if((ulValue & TIMER_KEY_1) != 0){

                 if(gpro_t.power_on == power_on)
                       g_tmsg.timer_sound_flag =1;
                
            }
            else if((ulValue & POWER_OFF_BIT_3) != 0){

                 gpro_t.power_on = power_off;
                      
                
            }
           
      }
      else{


             if(dc_power_on_first==0){
                 dc_power_on_first++;
                 HAL_Delay(1000);
                 VOICE_SOUND();

             }

              if(g_tmsg.power_onoff_sound_flag ==1){
                 g_tmsg.power_onoff_sound_flag++;
                 gpro_t.gTimer_set_minutes =0;
                 
                 if(gpro_t.power_on == power_off){
                    gpro_t.power_on = power_on;
                    gpro_t.works_2_hours_timeout_flag=0;
                    gpro_t.fan_warning_flag = 0;
                    gpro_t.gTimer_normal_run_main_function_times =10;
                    g_tmsg.works_mode_flag = ai_mode;
                    led_on_fun();
                    voice_power_on_sound();

                 }
                 else{

                   gpro_t.power_on = power_off;
                   led_off_fun();
                   voice_power_off_sound();
                 }
                 
             }

          if(g_tmsg.timer_sound_flag  ==1  &&  g_tmsg.key_long_timer_flag !=1 && g_tmsg.key_long_timer_flag !=2 && gpro_t.power_on == power_on){
                   g_tmsg.timer_sound_flag ++;
                   gpro_t.key_long_timer_counter=0;
                   set_timer_hold_flag=1;
                        
                       g_tmsg.works_mode_flag = timer_mode; //works mode two method. ai and timer .
                       
                       g_tmsg.key_timer_timing_value++ ;

                      if(g_tmsg.key_timer_timing_value > 24){
    
                            g_tmsg.key_timer_timing_value=1;
    
                       }
                      voice_set_timer_flag = 1;


                  
           }
           else if( g_tmsg.key_long_timer_flag == 1 && gpro_t.power_on == power_on){ //timer long be pressed is cacel timer function is ai_mode
                     g_tmsg.timer_sound_flag =0;
                    g_tmsg.key_long_timer_flag++;
                    gpro_t.gTimer_timer_time_long_key =0;
                    g_tmsg.works_mode_flag = ai_mode; //取消定时。
                
                     g_tmsg.key_timer_timing_value=0;
                      gpro_t.set_timer_timing_value=0;


           }

         if(gpro_t.power_on == power_on ){

           if(gpro_t.gTimer_timer_time_long_key > 1 &&  g_tmsg.key_long_timer_flag==2){
                g_tmsg.key_long_timer_flag=0;
                gpro_t.key_long_timer_counter=0;
               
           }
           if(voice_set_timer_flag == 1){
                 voice_set_timer_flag ++;
                  voice_set_timer_timing_value(g_tmsg.key_timer_timing_value);
                  gpro_t.set_timer_timing_value = g_tmsg.key_timer_timing_value;
                  gpro_t.gTimer_set_minutes = 0;
                   
            }
          
            main_board_ctl_handler(gpro_t.works_2_hours_timeout_flag,g_tmsg.works_mode_flag);
           
            device_works_time_counter_handler();

           // fan_adc_dma_fun();

             

          }
          else {
              gpro_t.works_2_hours_timeout_flag=0;

              g_tmsg.key_timer_timing_value=0;
              gpro_t.set_timer_timing_value =0;
              gpro_t.fan_warning_flag = 0;
              g_tmsg.works_mode_flag = timer_mode;
              power_off_handler();
              
             
          }

         
         
      }
             
    }
      
 }
/**********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
   //BaseType_t xResult;
   ///const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */

 
    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();
    if(KEY_POWER_GetValue()  == KEY_DOWN){
      
          xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
					 POWER_KEY_0,            /* 设置目标任务事件标志位bit0  */
					 eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/


    }
    else if(KEY_TIMER_GetValue() ==    KEY_DOWN ){

           gpro_t.key_long_timer_counter ++ ;
           if(gpro_t.power_on == power_on){
            led_on_fun();
           }
          if(gpro_t.key_long_timer_counter > 60 && gpro_t.power_on == power_on){
                
                gpro_t.key_long_timer_counter = 0;
               g_tmsg.key_long_timer_flag =1;
            
               voice_cancel_timer_timing();
               if(gpro_t.power_on == power_on){
               led_on_fun();
               }
             
          }
          else if(gpro_t.power_on == power_off){
          
          gpro_t.key_long_timer_counter=0;
          }

            if(g_tmsg.key_long_timer_flag ==0 &&  gpro_t.power_on == power_on){
                xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                TIMER_KEY_1,            /* 设置目标任务事件标志位bit0  */
                eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
        
            }


    }
  
    vTaskDelay(20);
  }
}
/**********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
static void AppTaskCreate (void)
{

  xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级次子*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级最高*/
                 &xHandleTaskStart );   /* 任务句柄  */
}

/*****************************************************************************
 * 
 * Function Name: void App_PowerOff_Handler(void)
 * Function:
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void App_PowerOff_Handler(void)
{
     
     xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
	 POWER_OFF_BIT_3 ,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}


