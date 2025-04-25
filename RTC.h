#ifndef __RTC_H
#define __RTC_H

#include <Arduino.h>
#include <Ds1302.h>

#define RTC_PIN_SCL D3
#define RTC_PIN_SDA D4
#define RTC_PIN_CE D5

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