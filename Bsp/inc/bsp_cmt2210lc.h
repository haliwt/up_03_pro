#ifndef __BSP_CMT2210LC_H
#define __BSP_CMT2210LC_H
#include "main.h"


#define RF_KEY_CMT2210LC_GetValue()    sys_gpio_pin_get(RF_KEY_GPIO_Port,RF_KEY_Pin)//HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)// sys_gpio_pin_get(RF_KEY_GPIO_Port,RF_KEY_Pin)  //  HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)   //


//#define RF_KEY_CMT2210LC_GetValue()     (((RF_KEY_GPIO_Port)->IDR & (1UL << (RF_KEY_Pin))) ? 1U : 0U)


void rfReceivedData_Handler(void);


#endif 

