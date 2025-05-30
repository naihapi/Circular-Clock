#include "Flash.h"

uint8_t Flash_ConfigState = 0;            // 0:未配置，1:配置完成
char Flash_WiFi_APSSID[32] = "WiFi-NAME"; // WiFi名称
char Flash_WiFi_APPWD[32] = "WiFi-PWD";   // WiFi密码
char Flash_WiFi_TOKEN[64] = "TOKEN";      // 用户密钥

/**
 * @brief 从闪存中取出数据
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Flash_Load(void)
{
    Flash_ConfigState = EEPROM.read(FLASH_ADDR_CONFIGSTATE);

    for (uint8_t i = 0; i < 32; i++)
    {
        Flash_WiFi_APSSID[i] = EEPROM.read(FLASH_ADDR_WIFI_APSSID + i);
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        Flash_WiFi_APPWD[i] = EEPROM.read(FLASH_ADDR_WIFI_APPWD + i);
    }

    for (uint8_t i = 0; i < 64; i++)
    {
        Flash_WiFi_TOKEN[i] = EEPROM.read(FLASH_ADDR_WIFI_TOKEN + i);
    }
}

/**
 * @brief 更新闪存数据
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Flash_Update(void)
{
    EEPROM.write(FLASH_ADDR_CONFIGSTATE, Flash_ConfigState);

    for (uint8_t i = 0; i < 32; i++)
    {
        EEPROM.write(FLASH_ADDR_WIFI_APSSID + i, Flash_WiFi_APSSID[i]);
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        EEPROM.write(FLASH_ADDR_WIFI_APPWD + i, Flash_WiFi_APPWD[i]);
    }

    for (uint8_t i = 0; i < 64; i++)
    {
        EEPROM.write(FLASH_ADDR_WIFI_TOKEN + i, Flash_WiFi_TOKEN[i]);
    }

    EEPROM.commit();
}

/**
 * @brief 闪存初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Flash_InitPro(void)
{
    EEPROM.begin(1024);

    // Flash_Update();
    Flash_Load();

    Serial.printf("%d %s %s %s\n", Flash_ConfigState, Flash_WiFi_APSSID, Flash_WiFi_APPWD, Flash_WiFi_TOKEN);
}
