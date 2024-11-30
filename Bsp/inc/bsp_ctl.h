#ifndef __BSP_CTL_H
#define __BSP_CTL_H
#include "main.h"



#define  UNIT_TESTING     1

typedef enum _AI_STATE{

   ai_mode,
   timer_mode,

}AI_STATE_T;



void main_board_ctl_handler(uint8_t cmd,uint8_t mode);




#endif 

