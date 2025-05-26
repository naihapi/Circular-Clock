#pragma once

#include <Arduino.h>
#include "WS2812.h"

#define LED_COLOR_UPPERLINK_MODE1 LightYellow
#define LED_COLOR_UPPERLINK_MODE2 Aquamarine
#define LED_COLOR_UPPERLINK_MODE3 Pink
#define LED_COLOR_UPPERLINK_MODE4 Orange
#define LED_COLOR_UPPERLINK_MODE5 Snow

#ifdef __cplusplus
#include "Connect.h"
#include "./RTC.h"
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
    void LEDBoard_DisplayBackGround(uint32_t color);
    void LEDBoard_DisplayGradualBackGround(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    void LEDBoard_DisplayPoint(uint32_t color);
    void LEDBoard_DrawImg_Beta(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *img, WS2812_t color);
    void LEDBoard_Clear(void);
    void LEDBoard_Function(void);
    void LEDBoard_DisplayWaiting(uint32_t wait_color, uint32_t background_color);

#ifdef __cplusplus
}
#endif
