#include "bsp.h"




/***************************************************************************
 *  *
    *Function Name: void main_board_ctl_handler(uint8_t flag,uint8_t cmd)
    *Function: flag: 休息10分钟， 还是在工作2个小时内
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
****************************************************************************/
void main_board_ctl_handler(uint8_t flag)
{
    //TODO
   
    if(flag == 0){ //works 2 hours times lable .
    

       
        if(gpro_t.gTimer_normal_run_main_function_times > 6){
            gpro_t.gTimer_normal_run_main_function_times=0;
           fan_output_fun();
            plasma_output_fun();
            ultra_output_fun();

         }
         
      }      

}



