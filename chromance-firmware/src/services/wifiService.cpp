#include "wifiService.h"

using namespace Chromance;

WiFiService::WiFiService(Logger* logger)
{
    this->logger = logger;
}

void WiFiService::Setup()
{
    this->Configure();
    this->RegisterEventHandlers();
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
    this->logger->Info("Configure WiFi");
    
    this->logger->Debug("Erase existing WiFi config");
    WiFi.disconnect(false, true);
    vTaskDelay(1000);
    
    if (WiFi.getMode() != WIFI_STA)
    {
        this->logger->Debug("Set WiFi to station mode");
        WiFi.mode(WIFI_STA);
    }
}

void WiFiService::RegisterEventHandlers()
{
    this->logger->Debug("Register WiFi event handlers");
    WiFi.onEvent
    (
        [this](WiFiEvent_t event, WiFiEventInfo_t info)
        {
            this->WiFiDisconnected(event, info);
        },
        WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED
    );
}

void WiFiService::Connect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    this->logger->Debug("WiFi connecting...");
    WiFi.begin(WifiSsid, WifiPassword);
    unsigned long start = millis();
    
    while
    (
        WiFi.status() != WL_CONNECTED &&
        millis() - start < WifiConnectionTimeout * 1000
    )
    {
        this->logger->Debug("WiFi connecting...");
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        this->logger->Warn("Reboot with reason: WiFi timeout");
        ESP.restart();
    }

    this->logger->Debug("WiFi connected");
    this->logger->Info("IP address: " + WiFi.localIP().toString());
}

void WiFiService::WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // This is not thread safe it seems
    this->logger->Warn("WiFi disconnected: " + String(info.wifi_sta_disconnected.reason));
}