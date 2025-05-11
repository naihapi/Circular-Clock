#include "LEDBoard.h"
#include "Connect.h"
#include "RTC.h"

WS2812_t WSBoard[8][24]; // 灯板缓冲数组

// 灯板数字
uint8_t LEDBoard_Number[10][3] = {
    {0x1F, 0x11, 0x1F}, // 0
    {0x00, 0x00, 0x1F}, // 1
    {0x1D, 0x15, 0x17}, // 2
    {0x15, 0x15, 0x1F}, // 3
    {0x07, 0x04, 0x1F}, // 4
    {0x17, 0x15, 0x1D}, // 5
    {0x1F, 0x15, 0x1D}, // 6
    {0x01, 0x01, 0x1F}, // 7
    {0x1F, 0x15, 0x1F}, // 8
    {0x17, 0x15, 0x1F}, // 9
};

// 灯板冒号
uint8_t LEDBoard_Colon[] = {
    0x00, 0x0A, 0x00 // ':'
};

// 灯板分割点
uint8_t LEDBoard_Point[] = {
    0x00, 0x04, 0x00 // '.'
};

/**
 * @brief 灯板初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_InitPro(void)
{
    LEDBoard_DisplayGradualBackGround(0xCD00CD, 0x8B0A50, 0x8B0A50, NavyBlue);
    LEDBoard_DisplayPoint(DarkOrange);
    LEDBoard_DisplayMonth(88, Yellow);
    LEDBoard_DisplayDay(88, Yellow);

    LEDBoard_Update();
}

/**
 * @brief 将数组载入到库函数中
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_ColorLoad(void)
{
    uint8_t num = 0;

    for (uint8_t col = 0; col <= 16; col += 8)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            for (uint8_t j = 0; j < 8; j++)
            {
                WS2812_SetOneLight(num, WSBoard[i][j + col]);
                num += 1;
            }
        }
    }
}

/**
 * @brief 灯板更新灯珠
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 仅调用函数后，灯珠颜色才会改变
 */
void LEDBoard_Update(void)
{
    LEDBoard_ColorLoad();
    WS2812_Update();
}

/**
 * @brief  灯板更新坐标灯珠
 *
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_SetPixel(uint8_t x, uint8_t y, WS2812_t color)
{
    if (x > 24 || y > 8)
        return;

    WSBoard[y][x] = color;
}

/**
 * @brief 灯板设置亮度
 *
 * @param brightness 亮度
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_SetBrightness(uint8_t brightness)
{
    WS2812_SetBrightness(brightness);
}

/**
 * @brief  获取单一个数字
 *
 * @param number 数字(0~9)
 *
 * @retval 返回数字的第1个字节
 *
 * @note 数字尺寸：3*5
 */
uint8_t *LEDBoard_GetNumberPointer(uint8_t number)
{
    if (number > 9)
        number = 0;
    return LEDBoard_Number[number];
}

/**
 * @brief  绘制一张图片
 *
 * @param x x轴
 * @param y y轴
 * @param width 宽度
 * @param height 高度
 * @param img 图片
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 列行式绘制，仅适用于列不超过8位的图片
 */
void LEDBoard_DrawImg(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *img, WS2812_t color)
{
    // 列(按像素)
    for (uint8_t col = 0; col < width; col++)
    {
        // 行(按字节)
        for (uint8_t row = 0; row < 8; row++)
        {
            if (x + col >= 24 || y + row >= 8)
                continue;

            uint8_t bit = img[col] >> row & 0x01;

            if (bit)
            {
                LEDBoard_SetPixel(x + col, y + row, color);
            }
        }
    }
}

/**
 * @brief  绘制一张图片
 *
 * @param x x轴
 * @param y y轴
 * @param width 宽度
 * @param height 高度
 * @param img 图片
 * @param color 颜色
 *
 * @retval 返回数字的第1个字节
 *
 * @note 适用于多位的图片
 * @note 逐列式绘制，工具取模时低位优先
 */
void LEDBoard_DrawImg_Beta(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *img, WS2812_t color)
{
    uint8_t LargeRow = (height + 7) / 8; // 1列的共计字节

    // 列（按像素）
    for (uint8_t col = 0; col < width; col++)
    {
        // 行（按像素）
        for (uint8_t row = 0; row < height; row++)
        {
            // 实际坐标
            uint8_t Real_X = x + col;
            uint8_t Real_Y = y + row;

            // 溢出处理（宽24高8的溢出处理）
            if (x + col >= 24 || y + row >= 8)
                continue;

            uint8_t CurrentByte = row / 8;  // 当前列的第CurrentByte个字节
            uint8_t CurrentPixel = row % 8; // CurrentByte个字节内的第CurrentPixel个像素

            uint8_t ImgByte = img[col * LargeRow + CurrentByte]; // 获取当前列的字节（通过共计字节、当前列获取首字节，再使用当前字节个数获取偏移字节）
            uint8_t ImgPixel = (ImgByte >> CurrentPixel) & 0x01; //  获取当前像素值

            if (ImgPixel)
            {
                LEDBoard_SetPixel(Real_X, Real_Y, color);
            }
        }
    }
}

/**
 * @brief  绘制1个数字
 *
 * @param x x坐标
 * @param y y坐标
 * @param number 数字(0~9)
 * @param color 颜色
 *
 * @retval 返回数字的第1个字节
 *
 * @note 无
 */
void LEDBoard_DrawNumber(uint8_t x, uint8_t y, uint8_t number, WS2812_t color)
{
    LEDBoard_DrawImg(x, y, 3, 5, LEDBoard_GetNumberPointer(number), color);
}

/**
 * @brief  绘制1个冒号
 *
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DrawColon(uint8_t x, uint8_t y, WS2812_t color)
{
    LEDBoard_DrawImg(x, y, 3, 5, LEDBoard_Colon, color);
}

/**
 * @brief  绘制1条水平线
 *
 * @param start_x x坐标
 * @param start_y y坐标
 * @param length 线长度
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DrawHorizontalLine(uint8_t start_x, uint8_t start_y, uint8_t length, WS2812_t color)
{
    for (uint8_t i = start_x; i <= length + start_x; i++)
    {
        LEDBoard_SetPixel(i, start_y, color);
    }
}

/**
 * @brief  绘制1条垂直线
 *
 * @param start_x x坐标
 * @param start_y y坐标
 * @param length 线长度
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DrawVerticalLine(uint8_t start_x, uint8_t start_y, uint8_t length, WS2812_t color)
{
    for (uint8_t i = start_y; i <= length + start_y; i++)
    {
        LEDBoard_SetPixel(start_x, i, color);
    }
}

/**
 * @brief  绘制小时
 *
 * @param hour 小时
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayHour(uint8_t hour, uint32_t color)
{
    LEDBoard_DrawNumber(3, 1, hour / 10, WS2812_GetColor(color));
    LEDBoard_DrawNumber(7, 1, hour % 10, WS2812_GetColor(color));
}

/**
 * @brief  绘制分钟
 *
 * @param minute 分钟
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayMinute(uint8_t minute, uint32_t color)
{
    LEDBoard_DrawNumber(13, 1, minute / 10, WS2812_GetColor(color));
    LEDBoard_DrawNumber(17, 1, minute % 10, WS2812_GetColor(color));
}

/**
 * @brief  绘制冒号
 *
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayColon(uint32_t color)
{
    LEDBoard_DrawColon(10, 1, WS2812_GetColor(color));
}

/**
 * @brief  绘制月份
 *
 * @param month 月份数
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayMonth(uint8_t month, uint32_t color)
{
    LEDBoard_DrawNumber(3, 1, month / 10, WS2812_GetColor(color));
    LEDBoard_DrawNumber(7, 1, month % 10, WS2812_GetColor(color));
}

/**
 * @brief  绘制日期
 *
 * @param day 天数
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayDay(uint8_t day, uint32_t color)
{
    LEDBoard_DrawNumber(13, 1, day / 10, WS2812_GetColor(color));
    LEDBoard_DrawNumber(17, 1, day % 10, WS2812_GetColor(color));
}

/**
 * @brief  绘制分割点
 *
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayPoint(uint32_t color)
{
    LEDBoard_DrawImg(10, 1, 3, 5, LEDBoard_Point, WS2812_GetColor(color));
}

/**
 * @brief  绘制状态条1
 *
 * @param now 当前值
 * @param max 最大值
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 屏幕右侧倒数第二列
 */
void LEDBoard_DisplayVerticalBar1(float now, float max, uint32_t color)
{
    LEDBoard_DrawVerticalLine(22, 0, now / max * 8.0, WS2812_GetColor(color));
}

/**
 * @brief  绘制状态条2
 *
 * @param now 当前值
 * @param max 最大值
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 屏幕右侧倒数第一列
 */
void LEDBoard_DisplayVerticalBar2(float now, float max, uint32_t color)
{
    LEDBoard_DrawVerticalLine(23, 0, now / max * 8, WS2812_GetColor(color));
}

/**
 * @brief  绘制背景色
 *
 * @param color 颜色
 *
 * @retval 无
 *
 * @note 需要在显示文字之前调用
 */
void LEDBoard_DisplayBackGround(uint32_t color)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        LEDBoard_DrawHorizontalLine(0, i, 23, WS2812_GetColor(color));
    }
}

/**
 * @brief  清空屏幕
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 关闭所有灯光
 */
void LEDBoard_Clear(void)
{
    LEDBoard_DisplayBackGround(0x00);
}

/**
 * @brief  绘制渐变背景色
 *
 * @param colorx 支持4种渐变色彩
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_DisplayGradualBackGround(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    LEDBoard_DrawHorizontalLine(0, 0, 23, WS2812_GetColor(color1));
    LEDBoard_DrawHorizontalLine(0, 1, 23, WS2812_GetColor(color1));

    LEDBoard_DrawHorizontalLine(0, 2, 23, WS2812_GetColor(color2));
    LEDBoard_DrawHorizontalLine(0, 3, 23, WS2812_GetColor(color2));

    LEDBoard_DrawHorizontalLine(0, 4, 23, WS2812_GetColor(color3));
    LEDBoard_DrawHorizontalLine(0, 5, 23, WS2812_GetColor(color3));

    LEDBoard_DrawHorizontalLine(0, 6, 23, WS2812_GetColor(color4));
    LEDBoard_DrawHorizontalLine(0, 7, 23, WS2812_GetColor(color4));
}

/**
 * @brief LED灯板控制函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void LEDBoard_Function(void)
{
    if (Connect_UpperControl == 1)
    {
        // 上位机控制
    }
    else if (Connect_UpperControl == 0)
    {
        // 正常显示时间
        LEDBoard_DisplayGradualBackGround(0xCD00CD, 0x8B0A50, 0x8B0A50, NavyBlue);
        LEDBoard_DisplayColon(DarkOrange);
        LEDBoard_DisplayMonth(RTC_DataBuffer[RTC_DATABUFFER_HOUR], Yellow);
        LEDBoard_DisplayDay(RTC_DataBuffer[RTC_DATABUFFER_MINUTE], Yellow);
    }

    LEDBoard_Update();
}
