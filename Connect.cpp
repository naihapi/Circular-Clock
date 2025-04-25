#include "Connect.h"

Ticker TIM;                    // 定时器对象(用于重置设备)
Ticker TIM_WiFiCollection;     // 定时器对象(用于WiFi监测)
uint8_t Connect_KEYCount = 0;  // 按键计数(用于重置设备)
uint8_t Connect_TIMCount = 0;  // 定时器计数(用于重置设备)
uint8_t Connect_ResetFlag = 0; // 设备重置标志

char Connect_ProductId[] = "CircularClock006"; // 产品ID
char Connect_DeviceType[] = "006";             // 设备类型(开关设备)
char Connect_DeviceName[] = "圆色时钟";        // 设备名称
char Connect_Version[] = "1";                  // 版本号

WiFiUDP MyUDP;                      // UDP对象
WiFiClient MyClient;                // 创建WiFi客户端对象
PubSubClient MyPSC(MyClient);       // 继承客户端对象
uint8_t Connect_CloudLink_Flag = 0; // 云端连接标志(0：未连接，1：已连接)

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

                // 置位并更新Flash
                Flash_ConfigState = 1;
                Flash_Update();

                return;
            }
        }
    }
}

/**
 * @brief 重置设备
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 清除用户配置信息
 */
void Connect_ResetDevice(void)
{
    Serial.printf("Device-RESET:Start\n");

    Serial.printf("Device-RESET:Clear Data\n");
    strcpy(Flash_WiFi_APSSID, "WiFi-NAME");
    strcpy(Flash_WiFi_APPWD, "WiFi-PWD");
    strcpy(Flash_WiFi_TOKEN, "TOKEN");
    Flash_ConfigState = 0;

    Serial.printf("Device-RESET:Flash Update Data\n");
    Flash_Update();

    Serial.printf("Device-RESET:Restart!\n\n");
    ESP.restart();
}

/**
 * @brief 定时器中断服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 在6s以内，按键次数超过5次，则重启设备
 */
void TIM_Handler(void)
{
    Connect_TIMCount++;

    // 定时器计次超过6下
    if (Connect_TIMCount >= 6)
    {
        // 按键计次超过5下
        if (Connect_KEYCount >= 5)
        {
            Serial.printf("Device-RESET:Count Over!\n");
            Connect_ResetDevice();
        }
        else
        {
            // 按键计次未超过5下
            Serial.printf("Device-RESET:Count Not Over!\n");
            TIM.detach();

            Connect_TIMCount = 0;
            Connect_KEYCount = 0;
        }
    }
}

/**
 * @brief 按键中断服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 按键按下超过五次，则重置连接
 */
IRAM_ATTR void D1_INTHandler()
{
    if (digitalRead(CONNECT_PIN) == LOW)
    {
        if (digitalRead(CONNECT_PIN) == LOW)
        {
            if (Connect_TIMCount == 0)
            {
                // 启动定时器，此函数仅执行一次
                Connect_TIMCount = 1;
                TIM.attach(1, TIM_Handler);
            }

            Connect_KEYCount++;
            Serial.printf("Device-RESET:Record Count-%d\n", Connect_KEYCount);
        }
    }
}

/**
 * @brief  按键初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 上拉+下降沿触发
 */
void Connect_KEY_Init(void)
{
    pinMode(CONNECT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CONNECT_PIN), D1_INTHandler, FALLING);
}

/**
 * @brief  WiFi连接
 *
 * @param ssid 名称
 * @param pwd 密码
 * @param time_out 超时次数(1s/次)
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_WiFiLink(char *ssid, char *pwd, uint8_t time_out)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, pwd);

    Serial.printf("WiFi Connecting");
    while (1)
    {
        if (WiFi.status() == WL_CONNECTED && WiFi.localIP() != INADDR_NONE)
        {
            Serial.printf("\nWiFi Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            break;
        }
        delay(1000);
        Serial.printf(".");

        if (time_out-- == 0)
        {
            Serial.printf("\nWiFi Connect Failed,Restart!\n");
            ESP.restart();
        }
    }
}

/**
 * @brief  连接到云平台
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_Cloud_Link(void)
{
    Connect_WiFiLink(Flash_WiFi_APSSID, Flash_WiFi_APPWD, 10);

    // 存在密钥时，连接到云平台
    MyPSC.setServer(CONNECT_CLOUD_URL, CONNECT_CLOUD_PORT);
    if (MyPSC.connect(Flash_WiFi_TOKEN))
    {
        MyPSC.subscribe(Connect_ProductId);
        MyPSC.publish(Connect_ProductId, "Device Online");
        Serial.printf("Cloud Link Success!\n");
    }
    else
    {
        Serial.printf("Cloud Link Failed!\n");
    }
}

/**
 * @brief 定时器服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note WiFi+MQTT监测
 */
void TIM_WiFiCollection_Handler(void)
{
    if (WiFi.status() != WL_CONNECTED || WiFi.localIP() == INADDR_NONE && Connect_CloudLink_Flag == 0)
    {
        Connect_CloudLink_Flag = 1;
    }

    if (Connect_CloudLink_Flag == 1)
    {
        Serial.printf("WiFi Disconnect,Retry...\n");
        WiFi.disconnect();
        WiFi.begin(Flash_WiFi_APSSID, Flash_WiFi_APPWD);

        if (MyPSC.connect(Flash_WiFi_TOKEN))
        {
            MyPSC.publish(Connect_ProductId, "Device Online");
            Connect_CloudLink_Flag = 0;
        }
    }
}

/**
 * @brief  连接初始化汇总
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_InitPro(void)
{
    // 无配置信息，进入配网模式
    if (Flash_ConfigState == 0)
    {
        Connect_APLink();
    }

    Connect_KEY_Init();   // 按键初始化
    Connect_Cloud_Link(); // 连接到云平台
    TIM_WiFiCollection.attach(10, TIM_WiFiCollection_Handler);
}
