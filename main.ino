#include "./main.h"

void setup()
{
    USART_InitPro();
    Flash_InitPro();
    WS2812_InitPro();
    Connect_InitPro();
    RTC_InitPro();
    Style_InitPro();
}

void loop()
{
    RTC_Function();
    LEDBoard_Function();
    Connect_Function();

    delay(1000);
}
