#ifndef __BSP_PLASMA_H
#define __BSP_PLASMA_H

#ifdef __cplusplus
extern "C" {
    
 class plasma_class{

  public:
    
    void plasma_output_fun(void);
    void plasma_stop_fun(void);



};

    
    
#endif

#include "main.h"


#define PLASMA_CTL_SetLow()      do{PLASMA_GPIO_Port->BRR = PLASMA_Pin;}while(0)  //HAL_GPIO_WritePin(FAN_CTL_GPIO_Port,FAN_CTL_Pin,GPIO_PIN_RESET);

#define PLASMA_CTL_SetHigh()    do{PLASMA_GPIO_Port->BSRR |= PLASMA_Pin;}while(0)





#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


