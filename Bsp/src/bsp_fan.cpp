#include "bsp.h"





void  fan_class::fan_output_fun(void)
{
  FAN_CTL_SetHigh();
}
void fan_class::fan_stop_fun(void)
{

  FAN_CTL_SetLow();

}





