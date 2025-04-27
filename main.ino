#include "./main.h"

void setup()
{
    USART_InitPro();
    Flash_InitPro();
    WS2812_InitPro();
    Connect_InitPro();
    RTC_InitPro();

    LEDBoard_DisplayGradualBackGround(0xCD00CD, 0x8B0A50, 0x8B0A50, NavyBlue);
    LEDBoard_DisplayPoint(DarkOrange);
    LEDBoard_DisplayMonth(10, Yellow);
    LEDBoard_DisplayDay(24, Yellow);

    LEDBoard_Update();
}

void loop()
{
    RTC_Function();
    Connect_Function();

    delay(1000);
}
