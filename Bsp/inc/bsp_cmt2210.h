#ifndef __BSP_CMT2210_H
#define __BSP_CMT2210_H


#ifdef __cplusplus
extern "C" {





#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"




void rf_irqhandler(void);


extern uint8_t rf_data[4]; 

uint8_t remote_scan(void);


#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */







