 #include "bsp.h"

#define ADC_BUF_LEN  8

#define VREF_MV     3300UL
#define ADC_MAX     4095UL
#define SHIFT_BITS  24UL   // 放大倍数（定点位数），常用 20~28


// 预计算比例系数：等效于 (VREF_MV / (ADC_MAX * ADC_BUF_LEN)) << SHIFT_BITS
#define ADC_DENOM     ((uint32_t)(ADC_MAX * ADC_BUF_LEN))
#define SCALE_K_AVG   ((uint32_t)((((uint64_t)VREF_MV << SHIFT_BITS) + (ADC_DENOM/2)) / ADC_DENOM))

static int ADC1_DMA_CaptureAvg_mV(uint16_t *mV_out);

uint16_t Get_ADC_Average_Voltage(uint16_t sum) ;
// 乘法 + 移位，避免运行时除法
//uint32_t tmp = (uint64_t)adc_raw * SCALE_K;
//uint16_t voltage_mv = tmp >> SHIFT_BITS;  // 单位 mV

//#define ADC_BUF_LEN  16
volatile uint16_t g_adc_buf[ADC_BUF_LEN];

uint16_t ADC_ConvertedValue;

uint32_t ADC_ConvertedValueLocal;

uint8_t fan_counter;


volatile uint16_t g_adc_buf[ADC_BUF_LEN];
uint16_t ADC1_ReadOnce(void);

void ADC_DMA_Polling(void)
{
     // 1. 开启时钟
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

    // 2. 校准（必须在使能前）
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));

    // 3. 配置触发源 = 软件触发
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);

    // 4. 配置通道（假设 CH1，采样时间可根据源阻抗调节）
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_39CYCLES_5);

    // 5. 使能 ADC
    LL_ADC_Enable(ADC1);
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1));
  
}


void  fan_adc_dma_fun(void)
{

  if(gpro_t.gTimer_adc_detected_time > 7  && gpro_t.works_2_hours_timeout_flag==0 && gpro_t.fan_warning_flag ==0){

      gpro_t.gTimer_adc_detected_time=0;

       // 启动转换（DMA 已在循环模式接收）
      // LL_ADC_REG_StartConversion(ADC1);

    /* 3.3为AD转换的参考电压值，stm32的AD转换为12bit，2^12=4096，
          即当输入为3.3V时，AD转换结果为4096 */    
      // ADC_ConvertedValueLocal =(uint32_t)((ADC_ConvertedValue)*3300)/4096;   // ADC_ConvertedValue只取最低12有效数据 放大倍数1000
     // ADC_ConvertedValueLocal = Get_ADC_Average_Voltage(g_adc_buf[0]) ;//adc_in0_read_mv_avg(); //获取均值
        ADC_ConvertedValue=ADC1_ReadOnce();
       ADC_ConvertedValueLocal= Get_ADC_Average_Voltage(ADC_ConvertedValue)  ;
      printf("ADC_VALUE0=%d\r\n",ADC_ConvertedValueLocal);
     // printf("ADC_VALUE1=%d\r\n",g_adc_buf[1]);
      
      if(ADC_ConvertedValueLocal< 86){
            
             fan_counter ++ ;    
         }

         if(fan_counter > 4){
          fan_counter =0;

          gpro_t.fan_warning_flag = 1;

         }

      }
  
   if(gpro_t.fan_warning_flag == 1 && gpro_t.gTimer_adc_detected_time > 3){

           gpro_t.gTimer_adc_detected_time=0;

          // Voice_Warning_Sound_Fan();


   }

}

// 获取缓冲区均值并换算毫伏（简单法：假设 VDD_MV 常数）
uint16_t adc_in0_read_mv_avg(void)
{
     
   //   // 配置DMA地址
   //  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
   //                        LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
   //                        (uint32_t)g_adc_buf,
   //                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    
   //  // 启用DMA中断
   // // LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
   // // NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
   // // NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    
   //  // 启用DMA通道
   //  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

   // // LL_DMA_SetPeriphAddress (DMA1, ch, (uint32_t)&ADC1->DR);
   // // LL_DMA_SetMemoryAddress (DMA1, ch, (uint32_t)g_adc_buf);
   // // LL_DMA_SetDataLength    (DMA1, ch, ADC_BUF_LEN);

   //  // 如需半传输/传输完成中断，在此使能 NVIC + LL_DMA_EnableIT_* 并在中断中处理
   //  LL_DMA_EnableChannel(DMA1, ch);

   //   // 启动ADC连续转换
   //  LL_ADC_REG_StartConversion(ADC1);

      // 读取结果
    //uint16_t value = LL_ADC_REG_ReadConversionData12(ADC1);
   #if 0
   uint32_t acc = 0;
    for (uint8_t i = 0; i < ADC_BUF_LEN; ++i) {
        acc += g_adc_buf[i];
    }

    // 定点乘法 + 移位，避免运行时除法
    uint64_t tmp = (uint64_t)acc * SCALE_K;
    return (uint16_t)(tmp >> SHIFT_BITS);

    #endif 
}




uint16_t Get_ADC_Average_Voltage(uint16_t sum) 
{
    const uint32_t K = 26424;  // (3300 << 16) / 4095
    return (sum * K) >> 16;
}

uint16_t ADC1_ReadOnce(void)
{
    LL_ADC_REG_StartConversion(ADC1);              // 软件启动
    while (!LL_ADC_IsActiveFlag_EOC(ADC1));        // 等待结束
    return LL_ADC_REG_ReadConversionData12(ADC1);  // 读取 12bit 数据
}

static int ADC1_DMA_CaptureAvg_mV(uint16_t *mV_out)
{
    // 1) 时钟
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    // 2) DMA 配置（禁用状态下）
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetChannelPriorityLevel (DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);
    LL_DMA_SetMode                 (DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);      // 单次搬运
    LL_DMA_SetPeriphIncMode        (DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode        (DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize           (DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetMemorySize           (DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_ConfigAddresses         (DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ADC1->DR, (uint32_t)g_adc_buf,
                                    LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetDataLength           (DMA1, LL_DMA_CHANNEL_1, ADC_BUF_LEN);

    // G0 需要设置 DMAMUX 请求源为 ADC1
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_ADC1);

    // 清标志
    LL_DMA_ClearFlag_GI1(DMA1);
    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_ClearFlag_HT1(DMA1);
    LL_DMA_ClearFlag_TE1(DMA1);

    // 3) ADC 配置：校准 → 通道 → 触发 → 使能
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));

    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
    LL_ADC_REG_SetSequencerRanks (ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1); // 换成你的通道
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_39CYCLES_5);

    LL_ADC_Enable(ADC1);
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1));

    // 开 ADC 的 DMA 请求：LIMITED 以匹配 NDTR 次数后触发 TC
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_LIMITED);

    // 4) 启动：先开 DMA，再启动转换（连续模式保证输出足够样本）
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
    LL_ADC_REG_StartConversion(ADC1);

    // 5) 轮询等待 TC（可加超时）
    uint32_t tmo = 2000000UL;
    while (!LL_DMA_IsActiveFlag_TC1(DMA1)) {
        if (tmo-- == 0) goto fail; // 超时保护
    }
    LL_DMA_ClearFlag_TC1(DMA1);

    // 6) 关 DMA（单次搬运结束），可视需要也停 ADC
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    // LL_ADC_REG_StopConversion(ADC1); // 如需

    // 7) 用缓冲计算平均并换算 mV（定点乘法，无除法）
    uint64_t sum = 0;
    for (uint32_t i = 0; i < ADC_BUF_LEN; ++i) sum += g_adc_buf[i];

    uint64_t acc = sum * SCALE_K_AVG;        // ~(sum*VREF)/(ADC_MAX*ADC_BUF_LEN) << SHIFT_BITS
    *mV_out = (uint16_t)(acc >> SHIFT_BITS); // mV
    return 0;

fail:
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    return -1;
}