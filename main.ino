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
    LEDBoard_Function();

    delay(1000);
}
