#include "bsp.h"

/*
 * File: rf_ev1527_decoder_ll.c
 * Target: STM32G030F6P6
 * RF Rx:  CMT2210LC DOUT -> PA6 (TIM3_CH1, AF1)
 * Timer:  TIM3 input capture @ 1 MHz (1 us/tick)
 * Timebase: TIM14 free-run @ 1 MHz for us timestamp (repeat/holdoff windows)
 * Toolchain: STM32Cube LL
 */

// ===================== 用户硬件定义 =====================
#define RF_GPIO_PORT           GPIOA
#define RF_GPIO_PIN            LL_GPIO_PIN_6      // PA6 -> TIM3_CH1 (AF1)

#define LED_PORT               GPIOA              // 可选：指示LED
#define LED_PIN                LL_GPIO_PIN_5

// ===================== 协议与阈值（单位：us） =====================
// 同步（低电平）≈ 13 ms
#define SYNC_MIN_US            12000
#define SYNC_MAX_US            15000

// 高电平宽度窗口（你示波器截图：bit0 ≈ 240–400 us）
#define T0_MIN_US              150
#define T0_MAX_US              450

// bit1 窗口（按经验 + 你提供的范围）
#define T1_MIN_US              500
#define T1_MAX_US              1100

// 其他
#define GLITCH_MIN_US          80      // 毛刺过滤
#define BITS_PER_FRAME         24
#define FRAME_TIMEOUT_US       20000   // 超过此间隔认为丢帧，复位解码（保守值）

// ===================== 重复包确认与抑制 =====================
// CMT2150A 默认连发8包，这里用 2 次一致确认即可（也可设为 3）
#define CONFIRM_THRESHOLD      2
#define REPEAT_WINDOW_US       60000   // 60 ms 内视为同一轮重复
#define HOLDOFF_US             150000  // 确认后 150 ms 内抑制相同帧

// ===================== 全局状态 =====================
typedef struct {
    volatile uint8_t  synced;      // 已检测同步
    volatile uint8_t  done;        // 完整一帧完成
    volatile uint8_t  bit_cnt;     // 已接收位数
    volatile uint32_t data;        // 24bit 数据
} rf_decoder_t;

static rf_decoder_t g_dec = {0};

// 当前通道极性（指“下一次要捕获的边沿”）
static uint32_t g_curr_pol = LL_TIM_IC_POLARITY_RISING;

// 重复帧过滤
typedef struct {
    uint32_t last_code;
    uint32_t last_time_us;
    uint8_t  repeat_cnt;

    uint32_t confirmed_code;
    uint32_t confirm_time_us;
    uint8_t  confirmed;            // 已上报
} rf_repeat_t;

static rf_repeat_t g_rep = {0};

// 微秒时间基（TIM14 1MHz 自由运行 + 溢出扩展）
static volatile uint32_t g_us_hi = 0; // 高位累加（每次溢出 + 65536）

static inline uint32_t now_us(void) {
    // 汇聚 32-bit 微秒：高位部分 + 当前CNT
    uint32_t hi1 = g_us_hi;
    uint16_t lo  = (uint16_t)LL_TIM_GetCounter(TIM14);
    uint32_t hi2 = g_us_hi;
    if (hi2 != hi1) {
        // 溢出竞争，再取一次
        lo = (uint16_t)LL_TIM_GetCounter(TIM14);
        hi1 = hi2;
    }
    return (hi1 << 16) | lo;
}

// ===================== GPIO & TIM 初始化 =====================
static void GPIO_RF_Init(void) {
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    // PA6 -> TIM3_CH1 AF1
    LL_GPIO_InitTypeDef gp = {0};
    gp.Pin        = RF_GPIO_PIN;
    gp.Mode       = LL_GPIO_MODE_ALTERNATE;
    gp.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    gp.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gp.Pull       = LL_GPIO_PULL_UP;          // 依据模块输出选择上/下拉；常见上拉更稳
    gp.Alternate  = LL_GPIO_AF_1;             // AF1 = TIM3_CH1 on PA6
    LL_GPIO_Init(RF_GPIO_PORT, &gp);

    // 可选LED
    gp.Pin        = LED_PIN;
    gp.Mode       = LL_GPIO_MODE_OUTPUT;
    gp.Pull       = LL_GPIO_PULL_NO;
    gp.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    LL_GPIO_Init(LED_PORT, &gp);
    LL_GPIO_ResetOutputPin(LED_PORT, LED_PIN);
}

static uint32_t TIM3_GetClockHz(void) {
    LL_RCC_ClocksTypeDef c;
    LL_RCC_GetSystemClocksFreq(&c);
    uint32_t pclk1 = c.PCLK1_Frequency;
    // APB1 分频 > 1 时，定时器时钟加倍
    uint32_t apbdiv = LL_RCC_GetAPB1Prescaler();
    uint32_t timclk = (apbdiv == LL_RCC_APB1_DIV_1) ? pclk1 : (pclk1 * 2U);
    return timclk;
}

static void TIM3_IC_1MHz_Init(void) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

    uint32_t timclk = TIM3_GetClockHz();
    uint32_t psc    = (timclk / 1000000U) - 1U; // 1 MHz
    LL_TIM_SetPrescaler(TIM3, (uint16_t)psc);
    LL_TIM_SetAutoReload(TIM3, 0xFFFF);
    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetCounter(TIM3, 0);

    // CH1: 直接输入，初始捕获上升沿
    LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
    LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
    // 数字滤波：FDIV8_N6（视噪声调大/调小）
    LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV8_N6);
    LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, g_curr_pol);

    // 中断：更新(超时复位) + CC1
    LL_TIM_EnableIT_UPDATE(TIM3);
    LL_TIM_EnableIT_CC1(TIM3);

    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM3);

    NVIC_SetPriority(TIM3_IRQn, 1);
    NVIC_EnableIRQ(TIM3_IRQn);
}

// TIM14 作为 1 MHz 微秒计数器（仅时间戳用，不影响捕获）
static uint32_t TIM14_GetClockHz(void) {
    LL_RCC_ClocksTypeDef c;
    LL_RCC_GetSystemClocksFreq(&c);
    // G0: TIM14 on APB2
    uint32_t pclk2 = c.PCLK2_Frequency;
    uint32_t apbdiv = LL_RCC_GetAPB2Prescaler();
    uint32_t timclk = (apbdiv == LL_RCC_APB2_DIV_1) ? pclk2 : (pclk2 * 2U);
    return timclk;
}

static void TIM14_UsClock_Init(void) {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM14);

    uint32_t timclk = TIM14_GetClockHz();
    uint32_t psc    = (timclk / 1000000U) - 1U; // 1 MHz
    LL_TIM_SetPrescaler(TIM14, (uint16_t)psc);
    LL_TIM_SetAutoReload(TIM14, 0xFFFF);
    LL_TIM_SetCounterMode(TIM14, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetCounter(TIM14, 0);

    // 仅用更新中断扩展 16->32 位
    LL_TIM_EnableIT_UPDATE(TIM14);
    LL_TIM_EnableCounter(TIM14);

    NVIC_SetPriority(TIM14_IRQn, 2);
    NVIC_EnableIRQ(TIM14_IRQn);
}

// ===================== 解码 & 过滤 =====================
static inline void RF_ResetDecoder(void) {
    g_dec.synced   = 0;
    g_dec.done     = 0;
    g_dec.bit_cnt  = 0;
    g_dec.data     = 0;
}

static inline void RF_ResetRepeat(void) {
    g_rep.last_code       = 0;
    g_rep.last_time_us    = 0;
    g_rep.repeat_cnt      = 0;
    g_rep.confirmed_code  = 0;
    g_rep.confirm_time_us = 0;
    g_rep.confirmed       = 0;
}

// 上层回调：确认一帧有效数据（去重后）
__attribute__((weak))
void RF_OnFrameConfirmed(uint32_t code24) {
    // 用户实际处理：解析地址/数据、执行业务
    // 例如：闪灯
    LL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

// 重复帧处理：2次一致确认 + 抑制窗口
static void RF_ProcessRepeat(uint32_t code24) {
    uint32_t t = now_us();

    if (code24 == g_rep.last_code && (t - g_rep.last_time_us) <= REPEAT_WINDOW_US) {
        g_rep.repeat_cnt++;
    } else {
        g_rep.last_code    = code24;
        g_rep.repeat_cnt   = 1;
    }
    g_rep.last_time_us = t;

    // 抑制窗口：在 holdoff 内同码直接忽略
    if (g_rep.confirmed && code24 == g_rep.confirmed_code && (t - g_rep.confirm_time_us) < HOLDOFF_US) {
        return;
    }

    if (g_rep.repeat_cnt >= CONFIRM_THRESHOLD) {
        // 确认新一轮
        g_rep.confirmed_code  = code24;
        g_rep.confirm_time_us = t;
        g_rep.confirmed       = 1;
        RF_OnFrameConfirmed(code24);
        // 不清 repeat_cnt，允许继续累积统计
    }
}

// ===================== 中断服务 =====================
void TIM14_IRQHandler(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM14)) {
        LL_TIM_ClearFlag_UPDATE(TIM14);
        g_us_hi++;
    }
}

void TIM3_IRQHandler(void) {
    // 超时：任意 UPDATE 事件都视为帧中断
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
        LL_TIM_ClearFlag_UPDATE(TIM3);
        // 若长时间无边沿触发，复位解码器
        RF_ResetDecoder();
    }

    // 过捕获保护
    if (LL_TIM_IsActiveFlag_CC1OVR(TIM3)) {
        LL_TIM_ClearFlag_CC1OVR(TIM3);
        RF_ResetDecoder();
    }

    if (LL_TIM_IsActiveFlag_CC1(TIM3)) {
        uint16_t width_us = LL_TIM_IC_GetCaptureCH1(TIM3);
        LL_TIM_ClearFlag_CC1(TIM3);

        // prev_pol 指示“刚刚捕获的是哪个边沿”
        uint32_t prev_pol = g_curr_pol;

        // 设置下一次捕获的边沿极性
        g_curr_pol = (g_curr_pol == LL_TIM_IC_POLARITY_RISING)
                   ? LL_TIM_IC_POLARITY_FALLING
                   : LL_TIM_IC_POLARITY_RISING;
        LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, g_curr_pol);

        // 重置计数器，为下一半段计时
        LL_TIM_SetCounter(TIM3, 0);

        // 已完成帧：等主循环处理后 RF_ResetDecoder()
        if (g_dec.done) return;

        // 过滤毛刺
        if (width_us < GLITCH_MIN_US) return;

        // 约定：
        // - prev_pol == RISING ：低电平结束，width_us 为“低电平宽度”
        // - prev_pol == FALLING：高电平结束，width_us 为“高电平宽度”
        if (prev_pol == LL_TIM_IC_POLARITY_RISING) {
            // 检测同步低电平
            if (width_us >= SYNC_MIN_US && width_us <= SYNC_MAX_US) {
                g_dec.synced  = 1;
                g_dec.bit_cnt = 0;
                g_dec.data    = 0;
            } else {
                // 若已同步但低电平异常，可根据需要选择复位
                if (g_dec.synced && width_us > FRAME_TIMEOUT_US) {
                    RF_ResetDecoder();
                }
            }
        } else {
            // 高电平段 -> 判 0/1
            if (!g_dec.synced) return;

            if (width_us >= T0_MIN_US && width_us <= T0_MAX_US) {
                g_dec.data = (g_dec.data << 1);
                g_dec.bit_cnt++;
            } else if (width_us >= T1_MIN_US && width_us <= T1_MAX_US) {
                g_dec.data = (g_dec.data << 1) | 1U;
                g_dec.bit_cnt++;
            } else {
                // 高电平宽度不在窗口，丢帧
                RF_ResetDecoder();
                return;
            }

            if (g_dec.bit_cnt >= BITS_PER_FRAME) {
                g_dec.done   = 1;
                g_dec.synced = 0;
            }
        }
    }
}

// ===================== 对外 API =====================
void RF_Decode_Init(void) {
    // 假定 SystemCoreClock 已配置并更新（LL_Init1msTick/SystemCoreClockUpdate）
    GPIO_RF_Init();
    TIM3_IC_1MHz_Init();
    TIM14_UsClock_Init();
    RF_ResetDecoder();
    RF_ResetRepeat();
}

void RF_Decode_Poll(void) {
    if (g_dec.done) {
        uint32_t code = g_dec.data;
        RF_ProcessRepeat(code);
        RF_ResetDecoder();
    }
}

// ===================== 示例：最小 main（仅示例） =====================
// 如你已有工程入口，可将 RF_Decode_Init() 放在初始化阶段，RF_Decode_Poll() 放在主循环
#ifdef DEMO_MAIN
static void SystemClock_Config_64MHzHSI(void) {
    // 使用 CubeMX/你现有 SystemClock_Config 即可，这里仅示例
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1) {}
    LL_RCC_HSI_SetCalibTrimming(LL_RCC_HSICALIBRATION_DEFAULT);
    // 直接用 HSI16*4=64MHz 的 PLL 亦可，这里略
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {}
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    SystemCoreClockUpdate();
    LL_Init1msTick(SystemCoreClock);
}

int main(void) {
    SystemClock_Config_64MHzHSI();
    RF_Decode_Init();

    while (1) {
        RF_Decode_Poll();
        // 你的其他任务...
    }
}
#endif


