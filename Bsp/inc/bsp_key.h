#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "main.h"



#define RF_KEY_GetValue()    HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)// sys_gpio_pin_get(RF_KEY_GPIO_Port,RF_KEY_Pin)  //  HAL_GPIO_ReadPin(RF_KEY_GPIO_Port,RF_KEY_Pin)   //
#define KEY_POWER_GetValue()        sys_gpio_pin_get(KEY_POWER_GPIO_Port,KEY_POWER_Pin)  //HAL_GPIO_ReadPin(KEY_ID_GPIO_Port,KEY_ID_Pin)   //

typedef enum _key_state{

   KEY_DOWN,
   KEY_UP,

}key_state;


uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx);


void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status);





#endif 

