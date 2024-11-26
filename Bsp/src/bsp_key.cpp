#include "bsp.h"




/**
 * @brief       set GPIO of state 
 * @param       p_gpiox: GPIOA~GPIOG, GPIO of pointer
 * @param       0X0000~0XFFFF, pinx of IO,
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1,  pin 
 *   @arg       0, output low level
 *   @arg       1, output high level
 * @retval      NO
 */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* set GPIO pinx = 1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* set GPIO pinx = 0 */
    }
}


/**
 * @brief       ¶ÁÈ¡GPIOÄ³¸öÒý½ÅµÄ×´Ì¬
 * @param       p_gpiox: GPIOA~GPIOG, GPIOÖ¸Õë
 * @param       0X0000~0XFFFF, Òý½ÅÎ»ÖÃ, Ã¿¸öÎ»´ú±íÒ»¸öIO, µÚ0Î»´ú±íPx0, µÚ1Î»´ú±íPx1, ÒÀ´ÎÀàÍÆ. ±ÈÈç0X0101, ´ú±íÍ¬Ê±ÉèÖÃPx0ºÍPx8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @retval      return value: 0 - low level, 1- high level
 */
uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx)
{
    if (p_gpiox->IDR & pinx)
    {
        return 1;   /* pinx - is low level*/
    }
    else
    {
        return 0;   /* pinx GPIO is high level */
    }
}


