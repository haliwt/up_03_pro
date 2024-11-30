#ifndef __BSP_FAN_H
#define __BSP_FAN_H
#include "main.h"



#define FAN_CTL_SetLow()      do{FAN_CTL_GPIO_Port->BRR = FAN_CTL_Pin;}while(0)  //HAL_GPIO_WritePin(FAN_CTL_GPIO_Port,FAN_CTL_Pin,GPIO_PIN_RESET);

#define FAN_CTL_SetHigh()    do{FAN_CTL_GPIO_Port->BSRR |= FAN_CTL_Pin;}while(0)


void fan_output_fun(void);
void fan_stop_fun(void);



#endif 

