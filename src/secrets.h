#ifndef SECRETS_H_
#define SECRETS_H_

#include <Arduino.h>
// Generated from environment variables by scripts/inject_secrets.py
#include "secretsEnv.h"

namespace Chromance
{
    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    constexpr const char* WifiSsid = CHROMANCE_WIFI_SSID;
    constexpr const char* WifiPassword = CHROMANCE_WIFI_PASSWORD;

    //////////////////////////////////////////
    // OTA
    //////////////////////////////////////////

    constexpr const char* OTAPassword = CHROMANCE_OTA_PASSWORD;

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    constexpr const char* MQTTBroker = CHROMANCE_MQTT_BROKER;
    constexpr int32_t MQTTPort = CHROMANCE_MQTT_PORT;
    constexpr const char* MQTTUsername = CHROMANCE_MQTT_USERNAME;
    constexpr const char* MQTTPassword = CHROMANCE_MQTT_PASSWORD;
}

#endif
