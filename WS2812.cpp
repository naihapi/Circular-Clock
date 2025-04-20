#include "WS2812.h"

Adafruit_NeoPixel led(WS2812_NUM, WS2812_PIN, NEO_GRB + NEO_KHZ800);

/**
 * @brief 灯珠引脚初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void WS2812_GPIO_Init(void)
{
    pinMode(WS2812_PIN, OUTPUT);
    digitalWrite(WS2812_PIN, 0);
}

/**
 * @brief 灯板更新
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void WS2812_Update(void)
{
    led.show();
}

/**
 * @brief 设置灯珠颜色
 *
 * @param led_num 第x个灯珠
 * @param color 第x个灯珠的颜色
 *
 * @retval 无
 *
 * @note 无
 */
void WS2812_SetOneLight(uint16_t led_num, WS2812_t color)
{
    led.setPixelColor(led_num, led.Color(color.Red, color.Green, color.Blue));
}

/**
 * @brief 所有灯珠颜色设置
 *
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void WS2812_SetAllLight(WS2812_t color)
{
    for (int i = 0; i < WS2812_NUM; i++)
    {
        led.setPixelColor(i, led.Color(color.Red, color.Green, color.Blue));
    }
}

void WS2812_SetBrightness(uint8_t brightness)
{
    led.setBrightness(brightness);
}

WS2812_t WS2812_GetColor(uint32_t hex_color)
{
    WS2812_t color;

    color.Red = (hex_color >> 16) & 0xFF;
    color.Green = (hex_color >> 8) & 0xFF;
    color.Blue = hex_color & 0xFF;

    return color;
}

/**
 * @brief 灯珠初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void WS2812_InitPro(void)
{
    led.begin();
    WS2812_SetBrightness(10);
    WS2812_GPIO_Init();
}
