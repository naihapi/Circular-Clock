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
#include <EEPROM.h>

// 自定义库
#include "Style.h"

// 闪存地址
#define FLASH_ADDR_CONFIGSTATE 0x00
#define FLASH_ADDR_WIFI_APSSID 0x01
#define FLASH_ADDR_WIFI_APPWD 0x21
#define FLASH_ADDR_WIFI_TOKEN 0x41
#define FLASH_ADDR_STYLEDIAL_CNT 0x81

#ifdef __cplusplus
extern "C"
{
#endif
    // 声明C兼容的函数（不能有C++特性）
    extern uint8_t Flash_ConfigState;
    extern uint8_t Flash_StyleDial;
    extern char Flash_WiFi_APSSID[32];
    extern char Flash_WiFi_APPWD[32];
    extern char Flash_WiFi_TOKEN[64];
    void Flash_Load(void);
    void Flash_Update(void);
    void Flash_InitPro(void);

#ifdef __cplusplus
}
#endif