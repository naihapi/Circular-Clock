#include "USART.h"

/**
 * @brief  串口初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void USART_InitPro(void)
{
    Serial.begin(115200);
    Serial.printf("Naihapi_CircularClock\n");
}
