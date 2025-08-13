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

    
}



