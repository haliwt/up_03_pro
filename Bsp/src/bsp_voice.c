#include "bsp.h"



#define MAX_BUFFER_SIZE  8
uint8_t outputBuf[MAX_BUFFER_SIZE];


static void voice_send_function_cmd(uint8_t cmd1,uint8_t cmd2);

static void send_tx_set_timer_value(uint8_t set_hours);

static void sendData_VoiceSound_Warning_Fan(void);

static void voice_send_power_on_cmd(void);


static void voice_send_power_off_cmd(void);




uint8_t transferSize;
static volatile uint8_t transOngoingFlag = 0;


/***************************************************************************
 *  *
    *Function Name: void voice_power_off_sound(void)
    *Function: 
    *Input Ref: NO
    *Return Ref:  NO
    * 
****************************************************************************/
void voice_power_on_sound(void)
{
   voice_send_power_on_cmd();

}
/***************************************************************************
 *  *
    *Function Name: void voice_power_off_sound(void)
    *Function: 
    *Input Ref: NO
    *Return Ref:  NO
    * 
****************************************************************************/
void voice_power_off_sound(void)
{
  voice_send_power_off_cmd();

}



/***************************************************************************
 *  *
    *Function Name: static void voice_set_timer_timing_value(uint8_t time)
    *Function: 
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
****************************************************************************/
void voice_set_timer_timing_value(uint8_t set_hours)
{
    
 


	  // set_hours = 4+ set_hours;
	   
	   send_tx_set_timer_value(set_hours);
   

      
}

/***************************************************************************
 *  *
    *Function Name: void voice_cancel_timer_timing(void)
    *Function: 
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
****************************************************************************/
void voice_cancel_timer_timing(void)
{
    
    voice_send_function_cmd(0x3F,0xE1);
}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void Voice_Warning_Sound_Fan(void)
{
	sendData_VoiceSound_Warning_Fan();
}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
static void sendData_VoiceSound_Warning_Fan(void)
{

	//crc=0x55;
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X41; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XE3;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
    HAL_UART_Transmit(&huart2,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 


}

/***************************************************************************
 *  *
    *Function Name: static void voice_send_function_cmd(uint8_t cmd1,uint8_t cmd2)
    *Function: 
    *Input Ref: 
    *Return Ref:  NO
    * 
****************************************************************************/
static void voice_send_function_cmd(uint8_t cmd1,uint8_t cmd2)
{
	
   outputBuf[0]=0xA5; //master
   outputBuf[1]=0XFA; //41
   outputBuf[2]=0X00; //44 // 'D' data
   outputBuf[3]=0X03; //   // 'R' rotator motor for select filter
   outputBuf[4]=cmd1; // // one command parameter
   outputBuf[5]=0X00;
   outputBuf[6]=cmd2;
   outputBuf[7]=0XFB;
   

   transferSize=8;

   HAL_UART_Transmit(&huart2,outputBuf,transferSize,0xffff);
   #if 0
   if(transferSize)
   {
	   while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
	   v_t.transOngoingFlag=1;
	   HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
   }
   #endif 



}
/*****************************************************************************
 * 
 * Function Name: static void send_tx_set_timer_value(uint8_t set_hours)
 * Function:
 * Input Ref: set timer timing time value
 * Return Ref:NO
 * 
*****************************************************************************/
static void send_tx_set_timer_value(uint8_t set_hours)
{

	switch(set_hours){
	

        case 1:
			voice_send_function_cmd(0x26,0xC8);
		break;
	
		case 2: //2 HOURS
			voice_send_function_cmd(0x27,0xC9);
		break;

        #if 0
		case 3: //2小时
			voice_send_function_cmd(0x28,0xCA);
		break;
        #endif 
	
		case 3://3
			voice_send_function_cmd(0x29,0xCB);
		break;
	
		case 4://4
			voice_send_function_cmd(0x2A,0xCC);
		break;
	
		case 5://5
			voice_send_function_cmd(0x2B,0xCD);
		break;
	
		case 6://6
			voice_send_function_cmd(0x2C,0xCE);
		   break;
	   
		case 7://7
		voice_send_function_cmd(0x2D,0xCF);
		break;
	
		case 8://8
		voice_send_function_cmd(0x2E,0xD0);
		break;
	
		case 9://9
		voice_send_function_cmd(0x2F,0xD1);
		break;
	
		case 10://10
		voice_send_function_cmd(0x30,0xD2);
		break;
	
		case 11://11
		voice_send_function_cmd(0x31,0xD3);
		break;
	
		case 12://12
		voice_send_function_cmd(0x32,0xD4);
		break;
	   
		case 13://13
		voice_send_function_cmd(0x33,0xD5);
		break;
	
		case 14://14
		voice_send_function_cmd(0x34,0xD6);
		break;
	
		case 15://15
		voice_send_function_cmd(0x35,0xD7);
		break;
	
		case 16://16
		voice_send_function_cmd(0x36,0xD8);
		break;
	
		case 17://17
		voice_send_function_cmd(0x37,0xD9);
		break;
	
	
		case 18://18
		voice_send_function_cmd(0x38,0xDA);
		break;
	
		case 19://19
		voice_send_function_cmd(0x39,0xDB);
		break;
	
		case 20://20
		voice_send_function_cmd(0x3A,0xDC);
		break;

		case 21://21
		voice_send_function_cmd(0x3B,0xDD);
		break;

		case 22://22
		voice_send_function_cmd(0x3C,0xDE);
		break;

		case 23://23
		voice_send_function_cmd(0x3D,0xDF);
		break;

		case 24://24
		voice_send_function_cmd(0x3E,0xE0);
		break;
	
		}

}




/********************************************************************************
	**
	*Function Name:static void voice_send_power_on_cmd(void)
	*Function :
	*Input Ref: NO
	*Return Ref:NO
	*
*******************************************************************************/
static void voice_send_power_on_cmd(void)
{
	
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00;
	outputBuf[3]=0X03; //44	// 'D' data
	outputBuf[4]=0X00; //	// 'R' rotator motor for select filter
	outputBuf[5]=0X00; // // one command parameter
	outputBuf[6]=0XA2;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
    //HAL_UART_Transmit(&huart2,outputBuf,transferSize,0xffff);
   
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart2,outputBuf,transferSize);
	}
   

}


static void voice_send_power_off_cmd(void)
{
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X01; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XA3;
	outputBuf[7]=0XFB;
	
	transferSize=8;
  
    #if 1
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart2,outputBuf,transferSize);
	}
    #else 
         HAL_UART_Transmit(&huart2,outputBuf,transferSize,0xffff);

    #endif 

}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart2) //voice  sound send 
	{
       //DISABLE_INT();

      // taskENTER_CRITICAL_FROM_ISR();
       transOngoingFlag=0; 
      // //ENABLE_INT();
      // taskEXIT_CRITICAL_FROM_ISR(0);
	}



}

