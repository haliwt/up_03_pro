#include "bsp.h"



void plasma_class :: plasma_output_fun(void)
{
    PLASMA_CTL_SetHigh();
}
void plasma_class:: plasma_stop_fun(void)
{

   PLASMA_CTL_SetLow();

}
    

