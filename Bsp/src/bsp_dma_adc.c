#include "bsp.h"

#define ADC_BUF_LEN_DMA  1  //  "1" has one ADC_CHANNEL_0 // (IN0?IN1)
uint16_t gdma_adc_buf[ADC_BUF_LEN_DMA];  // DMA???ADC?????
volatile uint16_t gdma_voltage[1];  

static uint16_t adc_to_mv(uint16_t raw_adc) ;
//__attribute__((aligned(4))) uint16_t gdma_adc_buf[2];// ?????????
uint16_t  fan_adc_counter;
uint8_t fan_counter ;
void DMA_ADC_Init(void)
{
    // 1. ????
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	


    // GPIO
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
   // LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

    // DMA
    //LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_ADC1);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1,LL_DMAMUX_REQ_ADC1);
    LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_1,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
        LL_DMA_MODE_NORMAL |
        LL_DMA_PERIPH_NOINCREMENT |
        LL_DMA_MEMORY_INCREMENT |
        LL_DMA_PDATAALIGN_HALFWORD |
        LL_DMA_MDATAALIGN_HALFWORD |
        LL_DMA_PRIORITY_HIGH);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
        (uint32_t)&ADC1->DR,
        (uint32_t)gdma_adc_buf,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
   // LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);//搬运两个数据,两个通道
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 1);
    // ADC
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));
    LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_LIMITED);
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_160CYCLES_5);
   // LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
   // LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_1);
    LL_ADC_Enable(ADC1);
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1));


}

/*@brief
    * @note   ADC_DMA_Polling
    * @param  None
    * @retval None
    */
void ADC_Read_DMA_Polling(void)
{

	 // ???
    LL_ADC_ClearFlag_EOS(ADC1);// 清除ADC1的"转换序列结束"（End Of Sequence, EOS）标志位。以下是详细解释：
    LL_DMA_ClearFlag_TC1(DMA1);//Transfer Complete, TC   CHANNEL_1

    // ?? DMA
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
   // LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);//DMA_CNDTRx（Channel x Number of Data Register）
   LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 1);//only one channel ADC_IN0
   LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    // ????
    LL_ADC_REG_StartConversion(ADC1);

    // ?? DMA ????
    while (!LL_DMA_IsActiveFlag_TC1(DMA1));

  

    // 3. ?????(VREF=3.3V)
    gdma_voltage[0] = adc_to_mv(gdma_adc_buf[0]);  // IN0??
//	printf("ADC_IN_0=%d\r\n",gdma_voltage[0]);
    //gdma_voltage[1] = (gdma_adc_buf[1] * 3300) / 4095;  // IN1??
	//printf("ADC_IN_1=%d\r\n",gdma_voltage[1]);

    // 4. ??DMA???
    // 4. ??DMA??
    LL_DMA_ClearFlag_TC1(DMA1);
}

/**
 * @brief  读取ADC电压值
 * @note   该函数会读取ADC转换后的电压值，并存储在全局变量gdma_voltage中。
 * @param  None 
 * @return None
 * @details 检测到0.2欧姆电阻
 */
void read_adc_dma_voltage(void)
{
    
    
	fan_adc_counter++;
	 if(fan_adc_counter > 400){ //400 ->8s
		 
      fan_adc_counter =0;
	   
       
	   ADC_Read_DMA_Polling();

           
      if(gdma_voltage[0]< 86){
            
             fan_counter ++ ;    
      }else if(gdma_voltage[0] > 86){

            fan_counter=0;

      }

        if(fan_counter > 4 && gpro_t.fan_warning_flag == 0){
          fan_counter =0;

          gpro_t.fan_warning_flag = 1;

         }

    
  
  
 
      
     }
      

   }


/**********************************************************
 * Function Name: compute_voltage
 * Function : multiplier = 3300 ¡Á (2^32 / 4095) ¡֠3462835200
 * Input Ref: raw_value - ADCԭʼֵ
 * 
 **********************************************************/
static uint16_t compute_voltage(uint16_t raw_value) 
{
    const uint32_t multiplier = 3462835200U;
    uint64_t temp = (uint64_t)raw_value * multiplier;
    return (uint16_t)(temp >> 32);  // µ¥λ£ºmV
}

/**********************************************************
 * 
 * Function Name: uint16_t adc_to_mv(uint16_t raw_adc) 
 * Function : multiplier = 3300 ¡Á (2^32 / 4095) ¡֠3462835200
 * Input Ref: raw_value - ADCԭʼֵ
 * Output Ref: mV - 计算后的电压值
 * 
 **********************************************************/
static uint16_t adc_to_mv(uint16_t raw_adc) 
{
    return (raw_adc * 26377UL) >> 15;  // 误差<0.1%
}

