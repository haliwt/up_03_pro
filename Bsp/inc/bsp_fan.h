#ifndef __BSP_FAN_H
#define __BSP_FAN_H

#ifdef __cplusplus
extern "C" {
    
    
class fan_class{

  public:

  void fan_output_fun(void);
  void fan_stop_fun(void);



};
   
#endif



#include "main.h"



#define FAN_CTL_SetLow()      do{FAN_CTL_GPIO_Port->BRR = FAN_CTL_Pin;}while(0)  //HAL_GPIO_WritePin(FAN_CTL_GPIO_Port,FAN_CTL_Pin,GPIO_PIN_RESET);

#define FAN_CTL_SetHigh()    do{FAN_CTL_GPIO_Port->BSRR |= FAN_CTL_Pin;}while(0)







#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */ 

