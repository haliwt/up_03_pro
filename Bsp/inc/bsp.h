#ifndef __BSP_H
#define __BSP_H


#include "main.h"

#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "bsp_ctl.h"
#include "bsp_adc.h"
#include "bsp_fan.h"
#include "bsp_ultrasonic.h"
#include "bsp_plasma.h"
#include "bsp_key.h"
#include "bsp_cmt2210.h"
#include "interrupt_manager.h"
#include "bsp_freertos.h"



#include "FreeRTOS.h"
#include "task.h"



#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	//#include "FreeRTOS.h"
	///#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum _power_on_state{

   power_off,
   power_on,

}power_on_state;


typedef struct process_t{


   uint8_t power_on;
   uint8_t setHours;
   uint8_t setMinutes;
   uint8_t works_2_hours_timeout_flag;

   uint8_t g_sync_flag;
   uint8_t receive_data_success;
   uint8_t stop_receive_data ;
   
   
 
   uint8_t fan_warning_flag;
   uint8_t rf_rec_data;
   uint8_t rf_rec_data1;
   uint8_t rf_rec_data2;

   uint8_t rf_rec_data_2;
   uint8_t rf_rec_data1_2;
   uint8_t rf_rec_data2_2;
   uint8_t second_low_level_adjsut;

   uint8_t  recieve_numbers;
 
   uint8_t  rf_key_interrupt_numbers ;

   uint8_t rf_auto_detected_num;
   uint8_t rf_receive_data_flag;
           
   
   uint16_t  low_level_getvalue;
   uint16_t  high_level_getvalue;

  
   //uint8_t gTimer_set_minutes;

   uint8_t gTimer_run_process;
   uint8_t gTimer_works_total_times_minutes;
   uint8_t gTime_rf_rc_data;
 
   uint8_t gTimer_stop_receive;
  
    
         
  

    


}PROCESS_T;

extern PROCESS_T  gpro_t;

void power_off_handler(void);

void device_works_time_counter_handler(void);




#endif /*__ GPIO_H__ */


