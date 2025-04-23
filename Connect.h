#pragma once

// 官方库
#include <lwip/ip_addr.h>
#include <lwip/opt.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// 宏定义
//...

// 自定义
#include "Flash.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 声明C兼容的函数（不能有C++特性）
    void Connect_InitPro(void);
    void Connect_APLink(void);
    void Connect_ResetDevice(void);

#ifdef __cplusplus
}
#endif
