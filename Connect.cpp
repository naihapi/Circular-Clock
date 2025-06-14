#include "Connect.h"

Ticker TIM;                           // 定时器对象(用于重置设备)
Ticker TIM_WiFiCollection;            // 定时器对象(用于WiFi监测)
Ticker TIM_UpperComputer;             //  定时器对象(用于与上位机通信)
uint8_t Connect_KEYCount = 0;         // 按键计数(用于重置设备)
uint8_t Connect_TIMCount = 0;         // 定时器计数(用于重置设备)
uint8_t Connect_ResetFlag = 0;        // 设备重置标志
uint8_t Connect_UpperControl = 0;     // 上位机控制标志(1：上位机控制，0：设备控制)
uint8_t Connect_UpperControl_Data[3]; //  上位机控制数据(CONNECT_UPPERDATA_xxx)

char Connect_ProductId[] = "CircularClock006"; // 产品ID
char Connect_DeviceType[] = "006";             // 设备类型(开关设备)
char Connect_DeviceName[] = "圆色时钟";        // 设备名称
char Connect_Version[] = "1";                  // 版本号

WiFiUDP MyUDP;                      // UDP对象
WiFiClient MyClient;                // 创建WiFi客户端对象
PubSubClient MyPSC(MyClient);       // 继承客户端对象
uint8_t Connect_CloudLink_Flag = 0; // 云端连接标志(1：连接正常，0：连接异常)
char Connect_RecBuffer[255];        // 接收缓冲区
uint32_t Connect_UNIXTime = 0;      // UNIX时间戳
uint8_t Connect_State = 0;          // 当前连接状态

/**
 * @brief  AP配网
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 使用WiFi+UDP进行配网
 * @note
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
                Flash_StyleDial = 1;   // 默认表盘
                Flash_ConfigState = 1; // 配置状态
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
 * @brief 重置按键初始化
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
 * @param time_out 超时次数(500ms/次)
 * @param restart_flag 超时重启(1重启，0不重启)
 * @retval 无
 *
 * @note 热点模式+WiFi模式 同时打开
 */
void Connect_WiFiLink(char *ssid, char *pwd, uint8_t time_out, uint8_t restart_flag)
{
    // WiFi连接
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, pwd);

    // 打开热点
    WiFi.softAP("CircularClock_Config"); // 热点名称
    MyUDP.begin(8266);                   // 监听8266端口

    Serial.printf("WiFi Connecting");
    while (1)
    {
        if (WiFi.status() == WL_CONNECTED && WiFi.localIP() != INADDR_NONE)
        {
            Connect_State = 1;
            Serial.printf("\nWiFi Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            break;
        }
        delay(500);
        Serial.printf(".");

        if (time_out-- == 0)
        {
            Serial.printf("\nWiFi Connect Failed!\n");
            Connect_State = 0;

            if (restart_flag == 1)
            {
                ESP.restart();
            }

            return;
        }
    }
}

/**
 * @brief 连接到云平台
 *
 * @param 无
 *
 * @retval 返回连接状态(1：连接成功，0：连接失败)
 *
 * @note 无
 */
uint8_t Connect_Cloud_Link(void)
{
    uint8_t flag = 0;

    // 存在密钥时，连接到云平台
    MyPSC.setServer(CONNECT_CLOUD_URL, CONNECT_CLOUD_PORT);
    if (MyPSC.connect(Flash_WiFi_TOKEN))
    {
        MyPSC.subscribe(Connect_ProductId);
        MyPSC.publish(Connect_ProductId, "Device Online");
        MyPSC.setKeepAlive(10);
        Serial.printf("Cloud Link Success!\n");

        flag = 1;
    }
    else
    {
        Serial.printf("Cloud Link Failed!\n");

        flag = 0;
    }

    return flag;
}

/**
 * @brief 定时器服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note WiFi+MQTT监测
 * @note 仅置标志位，不处理
 */
void TIM_WiFiCollection_Handler(void)
{
    // 依据官方文档，发送此消息即可保持连接
    MyPSC.publish(Connect_ProductId, "ping\r\n");

    if (WiFi.status() != WL_CONNECTED || WiFi.localIP() == INADDR_NONE && Connect_CloudLink_Flag == 0)
    {
        Connect_CloudLink_Flag = 1;
    }
}

/**
 * @brief 获取时间戳
 *
 * @param retry 重试次数(500ms/次)
 *
 * @retval 返回是否成功(1：成功，0：失败)
 *
 * @note 使用TCP协议获取时间戳(TCP和MQTT协议不能同时打开)
 * @note 时间戳获取到Connect_UNIXTime变量中
 */
uint8_t Connect_GetTimestamp(uint8_t retry)
{
    char str[256];
    char *p;
    uint8_t flag = 0;

    // TCP连接
    while (1)
    {
        if (MyClient.connect(CONNECT_CLOUD_URL, CONNECT_CLOUD_TCPPORT))
        {
            Serial.printf("TCP Connect Success\n");
            Connect_State = 1;
            break;
        }

        Serial.printf("TCP Connect Failed,Retry...\n");
        delay(500);

        if (retry-- == 0)
        {
            return flag;
        }
    }

    sprintf(str, "cmd=7&uid=%s&type=3\r\n", Flash_WiFi_TOKEN);
    MyClient.println(str);

    String line = MyClient.readStringUntil('\n');
    Serial.println(line.c_str());

    p = strstr(line.c_str(), "cmd=");
    *p = '\0';

    Connect_UNIXTime = atoi(line.c_str());
    Serial.printf("UNIX Convert:%d\n", Connect_UNIXTime);

    MyClient.stop();
    flag = 1;

    return flag;
}

/**
 * @brief 解析灯光数据
 *
 * @param msg 数据包
 * @param x x轴坐标
 * @param y y轴坐标
 * @param mode 灯光模式
 *
 * @retval 无
 *
 * @note 无
 */
void Connect_ParseMsg(char *msg, uint8_t *x, uint8_t *y, uint8_t *mode)
{
    sscanf(msg, "#upperlink#color#%hhu#%hhu#%hhu#", x, y, mode);
}

/**
 * @brief 解析灯光模式
 *
 * @param mode 灯光模式
 *
 * @retval 返回模式对应的颜色
 *
 * @note 无
 */
uint32_t Connect_ParseColorMode(uint8_t mode)
{
    uint32_t color = 0;

    switch (mode)
    {
    case 1:
    {
        color = LED_COLOR_UPPERLINK_MODE1;
    }
    break;
    case 2:
    {
        color = LED_COLOR_UPPERLINK_MODE2;
    }
    break;
    case 3:
    {
        color = LED_COLOR_UPPERLINK_MODE3;
    }
    break;
    case 4:
    {
        color = LED_COLOR_UPPERLINK_MODE4;
    }
    break;
    case 5:
    {
        color = LED_COLOR_UPPERLINK_MODE5;
    }
    break;
    }

    return color;
}

/**
 * @brief 解析上位机数据
 *
 * @param data 数据包
 * @param IPAddress IP地址
 * @param port 端口号
 *
 * @retval 解析结果(1：成功，0：失败)
 *
 * @note 进入绘制消息体(#upperlink#into)
 * @note 退出绘制消息体(#upperlink#back)
 * @note 灯光消息体(#upperlink#color#x坐标#y坐标#颜色模式#)：#upperlink#x#y#color_mode#
 */
uint8_t Connect_ParseUpperComputerData(char *data, IPAddress ip, uint16_t port)
{
    // 上位机连接数据包
    if (strstr(data, "upperlink#into") != NULL)
    {
        // 标志位置位
        LEDBoard_Clear();
        Connect_UpperControl = 1;

        // 回复上位机
        MyUDP.beginPacket(ip, port);
        MyUDP.write("ok");
        MyUDP.endPacket();
    }

    // 上位机断开数据包
    if (strstr(data, "upperlink#back") != NULL)
    {
        // 标志位置位
        Connect_UpperControl = 0;

        // 回复上位机
        MyUDP.beginPacket(ip, port);
        MyUDP.write("ok");
        MyUDP.endPacket();
    }

    // 上位机连接数据包
    if (strstr(data, "upperlink#into") != NULL)
    {
        // 标志位置位
        LEDBoard_Clear();
        Connect_UpperControl = 1;

        // 回复上位机
        MyUDP.beginPacket(ip, port);
        MyUDP.write("ok");
        MyUDP.endPacket();
    }

    // 清空屏幕
    if (strstr(data, "upperlink#clear") != NULL)
    {
        // 标志位置位
        LEDBoard_Clear();

        // 回复上位机
        MyUDP.beginPacket(ip, port);
        MyUDP.write("ok");
        MyUDP.endPacket();
    }

    // 灯板数据包
    if (strstr(data, "upperlink#color") != NULL && Connect_UpperControl == 1)
    {
        Connect_ParseMsg(data,
                         &Connect_UpperControl_Data[CONNECT_UPPERDATA_X],
                         &Connect_UpperControl_Data[CONNECT_UPPERDATA_Y],
                         &Connect_UpperControl_Data[CONNECT_UPPERDATA_MODE]);

        char str[128];
        sprintf(str, "APP send--%d %d %d", Connect_UpperControl_Data[0],
                Connect_UpperControl_Data[1], Connect_UpperControl_Data[2]);
        Serial.printf("%s\n", str);

        LEDBoard_SetPixel(Connect_UpperControl_Data[CONNECT_UPPERDATA_X],
                          Connect_UpperControl_Data[CONNECT_UPPERDATA_Y],
                          WS2812_GetColor(Connect_ParseColorMode(Connect_UpperControl_Data[CONNECT_UPPERDATA_MODE])));

        // 回复上位机
        MyUDP.beginPacket(ip, port);
        MyUDP.write("ok");
        MyUDP.endPacket();
    }

    return 1;
}

/**
 * @brief 上位机数据处理函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 仅有一个功能：举牌绘制
 * @note
 */
void TIM_UpperComputer_Handler(void)
{
    char PacketBuffer[255];
    uint8_t Length = 0;

    // 接收UDP数据包
    int PackSize = MyUDP.parsePacket();
    if (PackSize)
    {
        IPAddress RemoteIP = MyUDP.remoteIP();
        uint16_t Port = MyUDP.remotePort();

        Serial.printf("\nReceived Packet: %d Bytes\n", PackSize);
        Serial.printf("Remote IP: %s\n", RemoteIP.toString().c_str());
        Serial.printf("Remote Port: %u\n", Port);

        // 读取数据内容
        Length = MyUDP.read(PacketBuffer, 255);
        if (Length > 0)
        {
            PacketBuffer[Length] = '\0';
        }

        Connect_ParseUpperComputerData(PacketBuffer, RemoteIP, Port);
    }
}

/**
 * @brief 上位机连接初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 上位机为Android软件，通过UDP协议进行通讯
 */
void Connect_UpperInit(void)
{
    TIM_UpperComputer.attach(0.5, TIM_UpperComputer_Handler);
}

/**
 * @brief 连接功能处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 重连条件：秒钟为5~30时间段内、无设备连接、上位机未连接
 */
void Connect_Function(void)
{
    uint8_t connectNum = WiFi.softAPgetStationNum();
    // Serial.printf("connectNum=%d,Connect_CloudLink_Flag= %d,Connect_UpperControl= %d\n", connectNum, Connect_CloudLink_Flag, Connect_UpperControl);
    if (connectNum == 0 && Connect_CloudLink_Flag == 1 && Connect_UpperControl == 0 && RTC_DataBuffer[RTC_DATABUFFER_SECOND] < 30 && RTC_DataBuffer[RTC_DATABUFFER_SECOND] > 5)
    {
        Serial.printf("WiFi Disconnect,Retry...\n");

        Connect_WiFiLink(Flash_WiFi_APSSID, Flash_WiFi_APPWD, 10, 0); // 可重试时间5s

        // 先用TCP获取时间戳，再转为MQTT连接到云平台
        if (Connect_GetTimestamp(3) == 1)
        {
            RTC_TimeInit(Connect_UNIXTime, 1);
            if (Connect_Cloud_Link() == 1)
            {
                Connect_CloudLink_Flag = 0;
            }
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

    // 打开热点
    WiFi.softAP("CircularClock_Config"); // 热点名称
    MyUDP.begin(8266);                   // 监听8266端口

    Connect_KEY_Init();                                           // 重置按键初始化
    Connect_WiFiLink(Flash_WiFi_APSSID, Flash_WiFi_APPWD, 10, 0); // 连接WiFi
    Connect_GetTimestamp(3);                                      // 获取时间戳
    Connect_Cloud_Link();                                         // 连接到云平台
    Connect_UpperInit();                                          // 上位机连接初始化
    TIM_WiFiCollection.attach(5, TIM_WiFiCollection_Handler);     // 打开WiFi状态监测
}
