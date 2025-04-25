#include "RTC.h"

Ds1302 MyRTC(RTC_PIN_CE, RTC_PIN_SCL, RTC_PIN_SDA); // 创建对象
Ds1302::DateTime CurrentTIME;                       // 当前时间

/**
 * @brief  外部时钟初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void RTC_TimeInit(void)
{
    // 测试时钟是否已启动（未启动则现在启动）
    if (MyRTC.isHalted())
    {
        Ds1302::DateTime DT = {
            .year = 06,
            .month = Ds1302::MONTH_OCT,
            .day = 24,
            .hour = 10,
            .minute = 24,
            .second = 0,
            .dow = Ds1302::DOW_MON};

        MyRTC.setDateTime(&DT);
    }
}

/**
 * @brief  外部时钟功能函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 不能连续运行此函数
 */
void RTC_Function(void)
{
    MyRTC.getDateTime(&CurrentTIME);
    static uint8_t Second_Count = 0;

    if (Second_Count != CurrentTIME.second)
    {
        // 改变灯板数值
        //...

        Serial.printf("Date:%d-%d-%d\n", CurrentTIME.year, CurrentTIME.month, CurrentTIME.day);
        Serial.printf("Time:%d-%d-%d\n", CurrentTIME.hour, CurrentTIME.minute, CurrentTIME.second);
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
    RTC_TimeInit();
}
