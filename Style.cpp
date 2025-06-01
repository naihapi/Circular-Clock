#include "Style.h"

void Style_ConutFunction(void)
{
    Flash_StyleDial += 1;
    if (Flash_StyleDial > STYLE_DIALMAX)
    {
        Flash_StyleDial = 1;
    }
}

/**
 * @brief 按键中断服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
IRAM_ATTR void D6_INTHandler()
{
    if (!digitalRead(STYLE_Pin))
    {
        for (uint16_t i = 0; i < 500; i++)
        {
        }
        if (!digitalRead(STYLE_Pin))
        {
            for (uint8_t i = 0; i < 255; i++)
            {
            };
            if (!digitalRead(STYLE_Pin))
            {
                Style_ConutFunction();
                Flash_Update();
                Serial.printf("Device-MODE:%d\n", Flash_StyleDial);
            }
        }
    }
}

/**
 * @brief 表盘样式1
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 粉紫蓝渐变表盘
 */
void Style_ShowDial1(void)
{
    LEDBoard_DisplayGradualBackGround(0xCD00CD, 0xFF1493, 0x7323C2, 0x0C0CBA);
    LEDBoard_DisplayTime(RTC_DataBuffer[RTC_DATABUFFER_HOUR], RTC_DataBuffer[RTC_DATABUFFER_MINUTE], Yellow, Yellow);
}

/**
 * @brief 表盘样式2
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 纯白时钟无背景色
 */
void Style_ShowDial2(void)
{
    LEDBoard_DisplayGradualBackGround(0, 0, 0, 0);
    LEDBoard_DisplayTime(RTC_DataBuffer[RTC_DATABUFFER_HOUR], RTC_DataBuffer[RTC_DATABUFFER_MINUTE], White, White);
}

/**
 * @brief 表盘样式3
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 蓝色时间黄色底条
 */
void Style_ShowDial3(void)
{
    LEDBoard_DisplayGradualBackGround(0, 0, 0, Yellow);
    LEDBoard_DisplayTime(RTC_DataBuffer[RTC_DATABUFFER_HOUR], RTC_DataBuffer[RTC_DATABUFFER_MINUTE], Aquamarine, Aquamarine);
}

/**
 * @brief 表盘样式4
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 绿色时间白色底条
 */
void Style_ShowDial4(void)
{
    LEDBoard_DisplayGradualBackGround(0, 0, 0, LawnGreen);
    LEDBoard_DisplayTime(RTC_DataBuffer[RTC_DATABUFFER_HOUR], RTC_DataBuffer[RTC_DATABUFFER_MINUTE], LightSkyBlue, LightSkyBlue);
}

/**
 * @brief 表盘样式显示
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Style_ShowDial(void)
{
    switch (Flash_StyleDial)
    {
    case 1:
    {
        Style_ShowDial1();
    }
    break;
    case 2:
    {
        Style_ShowDial2();
    }
    break;
    case 3:
    {
        Style_ShowDial3();
    }
    break;
    case 4:
    {
        Style_ShowDial4();
    }
    break;
    }
}

/**
 * @brief 按键初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 上拉+下降沿触发
 */
void Style_KEY_Init(void)
{
    pinMode(STYLE_Pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(STYLE_Pin), D6_INTHandler, FALLING);
}

/**
 * @brief 样式初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Style_InitPro(void)
{
    Style_KEY_Init();
}
