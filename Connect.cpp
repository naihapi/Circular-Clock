#include "Connect.h"

SoftAP_t MySoftAP;
WiFiUDP MyUdp;

uint8_t Connect_WiFi_GetState(void)
{
    return 0;
}

void Connect_WiFi_GetIP(uint8_t *ip)
{
}

void Connect_WiFi_STAConfig(uint8_t *name, uint8_t *pwd) {}

void Connect_WiFi_APConfig(uint8_t *name, uint8_t *pwd, uint8_t channel, uint8_t hidden, uint8_t max_conn, uint32_t localport)
{
    WiFi.disconnect();

    MySoftAP.ssid = name;
    MySoftAP.pwd = pwd;
    MySoftAP.channel = channel;
    MySoftAP.hidden = hidden;
    MySoftAP.max_conn = max_conn;
    MySoftAP.localport = localport;

    WiFi.softAP(MySoftAP.ssid, MySoftAP.pwd, MySoftAP.channel, MySoftAP.hidden, MySoftAP.max_conn, MySoftAP.interval);
}

void Connect_WiFi_ModeConfig(WiFiMode_t mode)
{
}

void Connect_InitPro(void)
{
}
