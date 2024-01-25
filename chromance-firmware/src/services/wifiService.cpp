#include "wifiService.h"

using namespace Chromance;

WiFiService::WiFiService(Logger* logger)
{
    this->logger = logger;
}

void WiFiService::Setup()
{
    this->Configure();
    this->Connect();
}

void WiFiService::Loop()
{
    if (!WiFi.isConnected())
    {
        this->Connect();
    }
}

void WiFiService::Configure()
{
    WiFi.disconnect(false, true);
    vTaskDelay(1000);
    
    if (WiFi.getMode() != WIFI_STA)
    {
        WiFi.mode(WIFI_STA);
    }
}

void WiFiService::Connect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    WiFi.begin(WifiSsid, WifiPassword);
    unsigned long start = millis();
    
    while
    (
        WiFi.status() != WL_CONNECTED &&
        millis() - start < WifiConnectionTimeout * 1000
    )
    {
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        ESP.restart();
    }
}