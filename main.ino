#include "./main.h"

void setup()
{
    USART_InitPro();
    Flash_InitPro();
    WS2812_InitPro();
    Connect_InitPro();
    RTC_InitPro();
}

void loop()
{
    RTC_Function();
    Connect_Function();

    LEDBoard_DisplayGradualBackGround(0xCD00CD, 0x8B0A50, 0x8B0A50, NavyBlue);
    LEDBoard_DisplayPoint(DarkOrange);
    LEDBoard_DisplayMonth(RTC_DataBuffer[RTC_DATABUFFER_HOUR], Yellow);
    LEDBoard_DisplayDay(RTC_DataBuffer[RTC_DATABUFFER_MINUTE], Yellow);

    LEDBoard_Update();

    delay(1000);
}
