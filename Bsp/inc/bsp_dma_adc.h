#ifndef __BSP_DMA_ADC_H
#define __BSP_DMA_ADC_H
#include "main.h"


extern volatile uint16_t gdma_voltage[1];
void DMA_ADC_Init(void);
void read_adc_dma_voltage(void);


#endif 
