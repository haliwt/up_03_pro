#ifndef __INTERRUPT_MANAGER_H
#define __INTERRUPT_MANAGER_H
#include "main.h"


volatile extern uint32_t g_remote_data;

volatile extern uint8_t  rf_sync_signal_flag ;


void RF_ISR_receiveData(void);

void tim17_isr_callback(void);

#endif 

