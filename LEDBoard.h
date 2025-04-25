#pragma once

#include <Arduino.h>
#include "WS2812.h"

#ifdef __cplusplus
#include <Adafruit_NeoPixel.h>
extern "C"
{
#endif

    void LEDBoard_InitPro(void);
    void LEDBoard_Update(void);
    void LEDBoard_SetPixel(uint8_t x, uint8_t y, WS2812_t color);
    void LEDBoard_SetBrightness(uint8_t brightness);
    void LEDBoard_DrawImg(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *img, WS2812_t color);
    void LEDBoard_DrawNumber(uint8_t x, uint8_t y, uint8_t number, WS2812_t color);
    void LEDBoard_DrawHorizontalLine(uint8_t start_x, uint8_t start_y, uint8_t length, WS2812_t color);
    void LEDBoard_DrawVerticalLine(uint8_t start_x, uint8_t start_y, uint8_t length, WS2812_t color);
    void LEDBoard_DrawColon(uint8_t x, uint8_t y, WS2812_t color);
    void LEDBoard_DisplayVerticalBar1(float now, float max, uint32_t color);
    void LEDBoard_DisplayVerticalBar2(float now, float max, uint32_t color);
    void LEDBoard_DisplayColon(uint32_t color);
    void LEDBoard_DisplayHour(uint8_t hour, uint32_t color);
    void LEDBoard_DisplayMinute(uint8_t minute, uint32_t color);
    void LEDBoard_DisplayBackGround(uint32_t color);
    void LEDBoard_DisplayGradualBackGround(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    void LEDBoard_DisplayMonth(uint8_t month, uint32_t color);
    void LEDBoard_DisplayDay(uint8_t day, uint32_t color);
    void LEDBoard_DisplayPoint(uint32_t color);
    void LEDBoard_DrawImg_Beta(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *img, WS2812_t color);

#ifdef __cplusplus
}
#endif
