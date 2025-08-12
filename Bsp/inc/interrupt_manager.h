#ifndef __INTERRUPT_MANAGER_H
#define __INTERRUPT_MANAGER_H
#include "main.h"

volatile extern uint8_t rf_syn_flag ;
volatile extern uint32_t g_remote_data;

volatile extern uint8_t  rf_syn_flag ;

volatile extern uint32_t g_remote_data ; /* 红外接收到的数据 */
//extern uint8_t  g_remote_cnt ;  /* 按键按下的次敄1�7 */

//extern volatile uint8_t detected_rfSync_flag;

void RF_ResetDecoder(void);



#endif 

