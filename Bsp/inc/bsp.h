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
#include "bsp_voice.h"
#include "bsp_led.h"
#include "bsp_fan.h"
#include "bsp_ultrasonic.h"
#include "bsp_plasma.h"
#include "bsp_key.h"
#include "bsp_cmt2210lc.h"
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

   power_off=1,
   power_on,

}power_on_state;


typedef struct process_t{

   uint32_t  rf_id;

   uint8_t power_on;
   uint8_t setHours;
   uint8_t setMinutes;
   uint8_t works_2_hours_timeout_flag;

  

   uint8_t set_timer_timing_value;
   uint8_t fan_warning_flag;
   uint8_t rf_recieve_numbers;
 
   uint8_t powerOn_matchingId;
   uint8_t rf_syn_signal_numbers;
   uint8_t  rfPowerOnOff_soundFLag;

   uint8_t power_key_flag ;
   uint8_t power_on_off_numbers;
 


   uint8_t rf_receive_data_success;

   


  

   uint8_t gTimer_normal_run_main_function_times;
   uint8_t gTimer_timer_time_long_key ;
   uint8_t gTimer_works_total_times_minutes;
   uint8_t gTimer_adc_detected_time;
   uint8_t gTimer_power_on_times;
   uint8_t gTimer_switch_onoff;

    


}PROCESS_T;

extern PROCESS_T  gpro_t;

extern uint8_t checkRFCode_flag;

void power_off_handler(void);

void device_works_time_counter_handler(void);



void sound_power_on_off_handler(void);


#endif 


