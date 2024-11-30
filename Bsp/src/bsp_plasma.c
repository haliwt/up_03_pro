#include "bsp.h"



void plasma_output_fun(void)
{
    PLASMA_CTL_SetHigh();
}
void plasma_stop_fun(void)
{

   PLASMA_CTL_SetLow();

}
    

