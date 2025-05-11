#pragma once

/*由于ESP8266WiFi为C++库，所以需要声明为C兼容*/

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
#include <time.h>

// 宏定义
#define CONNECT_PIN D1
#define CONNECT_CLOUD_URL "bemfa.com"
#define CONNECT_CLOUD_PORT 9501    // 此为MQTT端口
#define CONNECT_CLOUD_TCPPORT 8344 // 此为TCP端口
#define CONNECT_UPPERDATA_X 0      // 上位机数据X坐标
#define CONNECT_UPPERDATA_Y 1      // 上位机数据Y坐标
#define CONNECT_UPPERDATA_MODE 2   // 上位机数据模式颜色

// 自定义
#include "LEDBoard.h"
#include "Flash.h"
#include "RTC.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 变量
    extern uint32_t Connect_UNIXTime;
    extern uint8_t Connect_State;
    extern uint8_t Connect_UpperControl;

    // 声明C兼容的函数（不能有C++特性）
    void Connect_InitPro(void);
    void Connect_APLink(void);
    void Connect_ResetDevice(void);
    void Connect_Function(void);

#ifdef __cplusplus
}
#endif
