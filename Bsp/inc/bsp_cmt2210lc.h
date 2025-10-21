#ifndef __BSP_CMT2210LC_H
#define __BSP_CMT2210LC_H
#include "main.h"


#define RF_KEY_Pin             LL_GPIO_PIN_6
#define RF_KEY_GPIO_Port       GPIOA


#define RF_KEY_CMT2210LC_GetValue() 		  LL_GPIO_IsInputPinSet(RF_KEY_GPIO_Port, RF_KEY_Pin)


//#define RF_KEY_CMT2210LC_GetValue()    sys_gpio_pin_get(RF_KEY_GPIO_Port,RF_KEY_Pin)//HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)// sys_gpio_pin_get(RF_KEY_GPIO_Port,RF_KEY_Pin)  //  HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)   //


//#define RF_KEY_CMT2210LC_GetValue()     (((RF_KEY_GPIO_Port)->IDR & (1UL << (RF_KEY_Pin))) ? 1U : 0U)

extern uint8_t pair_remote_ID_success_flag; 
void remoteControl_ID_firstPair433Mhz_handler(void);


#endif 

