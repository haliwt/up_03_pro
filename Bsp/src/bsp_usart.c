#include "bsp.h"

// 依据你的实际通道选择，下面以 DMA1 Channel1 为例
#define UART2_TX_DMA          DMA1
#define UART2_TX_CH           LL_DMA_CHANNEL_2
#define UART2_TX_TC_FLAG()    LL_DMA_IsActiveFlag_TC1(UART2_TX_DMA)
#define UART2_TX_CLEAR_TC()   LL_DMA_ClearFlag_TC1(UART2_TX_DMA)
#define UART2_TX_CLEAR_TE()   LL_DMA_ClearFlag_TE1(UART2_TX_DMA)
#define UART2_TX_CLEAR_GI()   LL_DMA_ClearFlag_GI1(UART2_TX_DMA)

void USART2_DMA_Send(uint8_t *data, uint16_t size)
{
   #if 0
// 1. 检查DMA是否空闲（防止覆盖未完成的传输）
    if (LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_2)) {
        return; // 或者等待/处理错误
    }
    
    // 2. 清除相关标志（TC和可能的错误标志）
    LL_DMA_ClearFlag_TC1(DMA1);    // 注意是 TC7（对应通道7）
    LL_DMA_ClearFlag_TE1(DMA1);    // 清除传输错误标志

      // 3. 配置DMA
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);                 // 先禁用通道
    LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_2, size);            // 设置数据长度
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, 
                          (uint32_t)data,                         // 源地址（内存）
                          LL_USART_DMA_GetRegAddr(USART2,LL_USART_DMA_REG_DATA_TRANSMIT),         // 目标地址（USART TDR）
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH);      // 传输方向
    LL_USART_EnableDMAReq_TX(USART2);                              // 使能USART DMA请求
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);                  // 启动DMA
    #endif

//	LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
//  
// 
//    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size);
//    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)data, (uint32_t)&USART2->TDR, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size);  
//	LL_USART_EnableDMAReq_TX(USART2);

//	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    #if 1
	if (data == NULL || size == 0) return ;

    // 若通道仍处于使能状态，先禁用并清标志，避免一直 return
    if (LL_DMA_IsEnabledChannel(UART2_TX_DMA, UART2_TX_CH)) {
        LL_DMA_DisableChannel(UART2_TX_DMA, UART2_TX_CH);
        // 可选：等待硬件确认关闭
        while (LL_DMA_IsEnabledChannel(UART2_TX_DMA, UART2_TX_CH)) {}
    }

    // 清 TC/TE 等标志
    UART2_TX_CLEAR_GI();

    // 配置地址与长度（内存为源，USART TDR 为目的）
    LL_DMA_ConfigAddresses(UART2_TX_DMA, UART2_TX_CH,
                           (uint32_t)data,
                           LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT),
                           LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetDataLength(UART2_TX_DMA, UART2_TX_CH, size);

    // 确保 USART 允许 DMA 请求（可放初始化中做一次）
    LL_USART_EnableDMAReq_TX(USART2);

    // 使能 DMA 通道，开始发送
    LL_DMA_EnableChannel(UART2_TX_DMA, UART2_TX_CH);

    #endif 
}



