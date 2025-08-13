#include "bsp.h"


void USART2_DMA_Send(uint8_t *data, uint16_t size)
{
       // 1. 检查DMA是否空闲（防止覆盖未完成的传输）
    if (LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1)) {
        return; // 或者等待/处理错误
    }
    
    // 2. 清除相关标志（TC和可能的错误标志）
    LL_DMA_ClearFlag_TC1(DMA1);    // 注意是 TC7（对应通道7）
    LL_DMA_ClearFlag_TE1(DMA1);    // 清除传输错误标志

      // 3. 配置DMA
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);                 // 先禁用通道
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size);            // 设置数据长度
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, 
                          (uint32_t)data,                         // 源地址（内存）
                          LL_USART_DMA_GetRegAddr(USART2,LL_USART_DMA_REG_DATA_TRANSMIT),         // 目标地址（USART TDR）
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH);      // 传输方向
    LL_USART_EnableDMAReq_TX(USART2);                              // 使能USART DMA请求
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);                  // 启动DMA
    
    //LL_DMA_ClearFlag_TC1(DMA1); // 清除传输完成标志
    //LL_USART_EnableDMAReq_TX(USART2);
    //LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size);
    //LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)data, (uint32_t)&USART2->TDR, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
   // LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}



