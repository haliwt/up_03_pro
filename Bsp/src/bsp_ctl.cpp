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

    fan_class fan_t;
    plasma_class plasma_t;
    ultra_class  ultra_t;
   
    if(flag == 0){ //works 2 hours times lable .
       
     
        if(gpro_t.gTimer_run_process >6){
            gpro_t.gTimer_run_process=0;
           
            plasma_t.plasma_output_fun();
            ultra_t.ultra_output_fun();

        }
       fan_t.fan_output_fun();
     

    }
    else{ //two 2 hours stop.
        
         fan_t.fan_stop_fun();
         plasma_t.plasma_stop_fun();
         ultra_t.ultra_stop_fun();


    }
}



