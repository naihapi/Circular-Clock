#include "Connect.h"

char Connect_ProductId[] = "CircularClock004"; // 产品ID
char Connect_DeviceType[] = "004";             // 设备类型
char Connect_DeviceName[] = "圆色时钟";        // 设备名称
char Connect_Version[] = "1";                  // 版本号
uint8_t Connect_ResetFlag = 0;                 // 设备重置标志
WiFiUDP MyUDP;

/**
 * @brief  AP配网
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_APLink(void)
{
    Serial.println("APLink Start, Waiting Connect...");

    WiFi.softAP("bemfa_CircularClock");
    MyUDP.begin(8266); // 监听8266端口
    while (1)
    {
        char PacketBuffer[255];
        uint8_t Length = 0;

        // 接收UDP数据包
        int PackSize = MyUDP.parsePacket();

        if (PackSize)
        {
            IPAddress RemoteIP = MyUDP.remoteIP();
            uint16_t Port = MyUDP.remotePort();

            Serial.printf("Received Packet: %d Bytes\n", PackSize);
            Serial.printf("Remote IP: %s\n", RemoteIP.toString().c_str());
            Serial.printf("Remote Port: %u\n", Port);

            // 读取数据内容
            Length = MyUDP.read(PacketBuffer, 255);
            if (Length > 0)
            {
                PacketBuffer[Length] = '\0';
            }

            Serial.printf("Rec-Data: %s\n", PacketBuffer);

            // 解析JSON数据
            StaticJsonDocument<200> Doc;
            DeserializationError Err = deserializeJson(Doc, PacketBuffer);
            if (Err)
            {
                Serial.printf("Parse-Json Failed: %s\n", Err.c_str());
                return;
            }

            // 处理命令类型
            int cmdType = Doc["cmdType"].as<int>();
            if (cmdType == 1)
            {
                // 提取配网信息
                const char *ssid = Doc["ssid"];
                const char *password = Doc["password"];
                const char *token = Doc["token"];

                Serial.printf("CMD Type: %d\n", cmdType);
                Serial.printf("SSID: %s\n", ssid);
                Serial.printf("Password: %s\n", password);
                Serial.printf("Token: %s\n", token);

                // 保存配置
                strcpy(Flash_WiFi_APSSID, ssid);
                strcpy(Flash_WiFi_APPWD, password);
                strcpy(Flash_WiFi_TOKEN, token);
                Flash_Update();

                // 构造回复信息
                String ReplyBuffer = "{\"cmdType\":2,\"productId\":\"" + String(Connect_ProductId) +
                                     "\",\"deviceName\":\"" + String(Connect_DeviceName) +
                                     "\",\"protoVersion\":\"" + String(Connect_Version) + "\"}";

                // 发送回复
                MyUDP.beginPacket(RemoteIP, Port);
                MyUDP.write(ReplyBuffer.c_str());
                MyUDP.endPacket();
            }
            else if (cmdType == 3)
            {
                // 结束配网模式
                WiFi.softAPdisconnect(true);
                Serial.println("AP Link Finished!");

                Flash_ConfigState = 1;
                return;
            }
        }
    }
}

/**
 * @brief  重置设备
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_ResetDevice(void)
{
    Serial.printf("Device-RESET:Start\n");

    Serial.printf("Device-RESET:Clear Flash Data\n");
    strcpy(Flash_WiFi_APSSID, "WiFi-NAME");
    strcpy(Flash_WiFi_APPWD, "WiFi-PWD");
    strcpy(Flash_WiFi_TOKEN, "TOKEN");
    Flash_ConfigState = 0;

    Serial.printf("Device-RESET:Flash Update Data\n");
    Flash_Update();

    Serial.printf("Device-RESET:Restart Finished\n\n");
    Connect_APLink();
}

/**
 * @brief  AP配网
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_InitPro(void)
{
    if (Flash_ConfigState == 0)
    {
        Connect_APLink();
    }
}
