#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "main.h"


#define GPIO_1_SetLow()        do{GPIO_1_GPIO_Port ->BRR =GPIO_1_Pin;}while(0)
#define GPIO_1_SetHigh()       do{GPIO_1_GPIO_Port ->BSRR |= GPIO_1_Pin;}while(0)




typedef enum _led_state{

    LED_ON,
    LED_OFF,

}led_state;

void led_on_fun(void);
void led_off_fun(void);


#endif 

