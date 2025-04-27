#include "RTC.h"

Ds1302 MyRTC(RTC_PIN_CE, RTC_PIN_SCL, RTC_PIN_SDA); // 创建对象
Ds1302::DateTime CurrentTIME;                       // 当前时间
uint8_t RTC_DataBuffer[6];                          // 数据存储(RTC_DATABUFFER_xxx)

/**
 * @brief  外部时钟初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 默认24小时制
 */
void RTC_TimeInit(time_t unix)
{
    setenv("TZ", "CST-8", 1); // CST = China Standard Time (UTC+8)
    tzset();                  // 应用时区设置
    struct tm *local_time = localtime(&unix);

    if (local_time == NULL)
    {
        Serial.printf("Unix Into RTC Failed\n");
        return;
    }

    // 提取时间分量
    int year = local_time->tm_year + 1900; // 完整年份（如 2024）
    int month = local_time->tm_mon + 1;    // 1-12
    int day = local_time->tm_mday;         // 1-31
    int hour = local_time->tm_hour;        // 0-23
    int minute = local_time->tm_min;       // 0-59
    int second = local_time->tm_sec;       // 0-59
    int wday = local_time->tm_wday;        // 0-6（0=周日）

    Serial.printf("%d-%d-%d %d:%d:%d\n", year, month, day, hour, minute, second);

    // 检查 RTC 是否停止
    if (MyRTC.isHalted())
    {
        // 转换星期为 Ds1302 的枚举格式（假设 tm_wday=0 是周日）
        Ds1302::DOW dow;
        switch (wday)
        {
        case 0:
            dow = Ds1302::DOW_SUN;
            break;
        case 1:
            dow = Ds1302::DOW_MON;
            break;
        case 2:
            dow = Ds1302::DOW_TUE;
            break;
        case 3:
            dow = Ds1302::DOW_WED;
            break;
        case 4:
            dow = Ds1302::DOW_THU;
            break;
        case 5:
            dow = Ds1302::DOW_FRI;
            break;
        case 6:
            dow = Ds1302::DOW_SAT;
            break;
        default:
            dow = Ds1302::DOW_MON; // 默认周一
        }

        // 转换月份为 Ds1302 的枚举格式
        Ds1302::MONTH rtc_month;
        switch (month)
        {
        case 1:
            rtc_month = Ds1302::MONTH_JAN;
            break;
        case 2:
            rtc_month = Ds1302::MONTH_FEB;
            break;
        case 3:
            rtc_month = Ds1302::MONTH_MAR;
            break;
        case 4:
            rtc_month = Ds1302::MONTH_APR;
            break;
        case 5:
            rtc_month = Ds1302::MONTH_MAY;
            break;
        case 6:
            rtc_month = Ds1302::MONTH_JUN;
            break;
        case 7:
            rtc_month = Ds1302::MONTH_JUL;
            break;
        case 8:
            rtc_month = Ds1302::MONTH_AUG;
            break;
        case 9:
            rtc_month = Ds1302::MONTH_SEP;
            break;
        case 10:
            rtc_month = Ds1302::MONTH_OCT;
            break;
        case 11:
            rtc_month = Ds1302::MONTH_NOV;
            break;
        case 12:
            rtc_month = Ds1302::MONTH_DEC;
            break;
        default:
            rtc_month = Ds1302::MONTH_JAN; // 默认一月
        }

        // 设置 RTC 时间
        Ds1302::DateTime DT = {
            .year = static_cast<uint8_t>(year % 100), // 取后两位（如 24）
            .month = rtc_month,
            .day = static_cast<uint8_t>(day),
            .hour = static_cast<uint8_t>(hour),
            .minute = static_cast<uint8_t>(minute),
            .second = static_cast<uint8_t>(second),
            .dow = dow};

        MyRTC.setDateTime(&DT);
        Serial.println("RTC 时间已更新！");
    }
}

/**
 * @brief  外部时钟功能函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 不能连续运行此函数，建议1s运行一次
 */
void RTC_Function(void)
{
    MyRTC.getDateTime(&CurrentTIME);
    static uint8_t Second_Count = 0;

    if (Second_Count != CurrentTIME.second)
    {
        // 刷新数据
        RTC_DataBuffer[0] = CurrentTIME.year;
        RTC_DataBuffer[1] = CurrentTIME.month;
        RTC_DataBuffer[2] = CurrentTIME.day;
        RTC_DataBuffer[3] = CurrentTIME.hour;
        RTC_DataBuffer[4] = CurrentTIME.minute;
        RTC_DataBuffer[5] = CurrentTIME.second;

        Serial.printf("Date:%d-%d-%d\n", RTC_DataBuffer[0], RTC_DataBuffer[1], RTC_DataBuffer[2]);
        Serial.printf("Time:%02d-%d-%d\n", RTC_DataBuffer[3], RTC_DataBuffer[4], RTC_DataBuffer[5]);
    }
}

/**
 * @brief  外部时钟初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void RTC_InitPro(void)
{
    MyRTC.init();
    RTC_TimeInit(Connect_UNIXTime);
}
