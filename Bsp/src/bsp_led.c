#include "bsp.h"


void led_on_fun(void)
{
   GPIO_1_SetLow(); //GPIO_1_SetHigh();
}

void led_off_fun(void)
{
   GPIO_1_SetHigh(); //GPIO_1_SetLow();
}
