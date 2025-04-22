#pragma once

#include <lwip/ip_addr.h>
#include <lwip/opt.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define CONNECT_WIFISSID "bemfa_CircularClock"
#define CONNECT_WIFIPWD "12345678"

typedef struct SoftAP_t
{
    uint8_t ssid[32];   // 名称
    uint8_t pwd[32];    // 密码
    uint8_t channel;    // 通道1~13
    uint8_t hidden;     // 隐藏
    uint8_t max_conn;   // 最大连接数
    uint8_t interval;   // 心跳间隔
    uint32_t localport; // 本地端口
};

typedef struct SoftSTA_t
{
    uint8_t ssid[32]; // 名称
    uint8_t pwd[32];  // 密码
};

#ifdef __cplusplus
extern "C"
{
#endif
    // 声明C兼容的函数（不能有C++特性）
    void Connect_InitPro(void);
    uint8_t Connect_WiFi_GetState(void);
    void Connect_WiFi_GetIP(uint8_t *ip);
    void Connect_WiFi_Link(uint8_t *name, uint8_t *pwd);

#ifdef __cplusplus
}
#endif
