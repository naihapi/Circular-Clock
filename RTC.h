#ifndef __RTC_H
#define __RTC_H

// 官方库
#include <Arduino.h>
#include <Ds1302.h>

// 自定义库
#include "Connect.h"

#define RTC_PIN_SCL D3
#define RTC_PIN_SDA D4
#define RTC_PIN_CE D5
#define RTC_DATABUFFER_YEAR 0
#define RTC_DATABUFFER_MONTH 1
#define RTC_DATABUFFER_DAY 2
#define RTC_DATABUFFER_HOUR 3
#define RTC_DATABUFFER_MINUTE 4
#define RTC_DATABUFFER_SECOND 5

#ifdef __cplusplus
extern "C"
{
#endif
    void RTC_InitPro(void);
    void RTC_Function(void);
#ifdef __cplusplus
}
#endif

#endif
