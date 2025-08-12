#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "main.h"


//#define KEY_POWER_GPIO_Port    GPIOA
//#define KEY_POWER_Pin          LL_GPIO_PIN_3
#define KEY_POWER_GetValue()   LL_GPIO_IsInputPinSet(KEY_POWER_GPIO_Port, KEY_POWER_Pin)



//#define KEY_POWER_GetValue()   sys_gpio_pin_get(KEY_POWER_GPIO_Port,KEY_POWER_Pin)  //HAL_GPIO_ReadPin(KEY_ID_GPIO_Port,KEY_ID_Pin)   //

//#define KEY_POWER_GetValue()    (((KEY_POWER_GPIO_Port)->IDR , (1UL << (KEY_POWER_Pin))) ? 1U : 0U)


typedef enum _key_state{

   KEY_DOWN,
   KEY_UP,

}key_state;








#endif 

