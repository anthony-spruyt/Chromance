#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <Arduino.h>

namespace Chromance
{
    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    /// @brief The connection timeout in seconds before reboot
    static const uint8_t WifiConnectionTimeout = 15;

    //////////////////////////////////////////
    // TIME
    //////////////////////////////////////////

    /// @brief The timeout in seconds for syncing internet time
    static const uint16_t NTPSyncTimeout = 10;
    /// @brief Local timezone
    static const String TimeZoneLocation = "Australia/Melbourne";

    //////////////////////////////////////////
    // OTA
    //////////////////////////////////////////

    /// @brief The OTA hostname - Note the setup function suffixes the chip ID
    static const char* Hostname = "Chromance";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    /// @brief The incoming topic
    static const char* MQTTTopic = "chromance/v1";
    /// @brief The incoming JSON payload type key
    static const char* MQTTPayloadTypeKey = "type";
    /// @brief The incoming JSON payload body key
    static const char* MQTTPayloadBodyKey = "body";

    //////////////////////////////////////////
    // CONFIG
    //////////////////////////////////////////

    /// @brief The preferences namespace
    static const char* ConfigNamespace = "config";
    /// @brief The preferences log level key
    static const char* LogLevelConfigKey = "ll";
    /// @brief They preferences brightness key
    static const char* BrightnessConfigKey = "l";
    /// @brief They preferences speed key
    static const char* SpeedConfigKey = "v";

    //////////////////////////////////////////
    // LEDs
    //////////////////////////////////////////

    static const int16_t BlueStripDataPin = 33;
    static const int16_t GreenStripDataPin = 27;
    static const int16_t RedStripDataPin = 2;
    static const int16_t BlackStripDataPin = 4;
    static const uint16_t BlueStripOffset = 0;
    static const uint16_t BlueStripLength = 154;
    static const uint16_t GreenStripOffset = BlueStripLength;
    static const uint16_t GreenStripLength = 168;
    static const uint16_t RedStripOffset = GreenStripLength + BlueStripLength;
    static const uint16_t RedStripLength = 84;
    static const uint16_t BlackStripOffset = RedStripLength + GreenStripLength + BlueStripLength;
    static const uint16_t BlackStripLength = 154;
    static const uint16_t NumberOfLEDs = BlueStripLength + GreenStripLength + RedStripLength + BlackStripLength;
    static const uint16_t MaxRefreshRate = 30;
    static const uint8_t StartupBrightness = 1;
    static const unsigned long StartupDelay = 5000;

    //////////////////////////////////////////
    // ANIMATIONS
    //////////////////////////////////////////

    /// @brief The duration in milliseconds when in random animation mode that each animation is played before transitioning to the next randomly selected animation
    static const unsigned long RandomAnimationDuration = 30000;
    static const uint8_t AnimationTransitionSpeed = 5;

    //////////////////////////////////////////
    // ANIMATIONS - RAINBOW
    //////////////////////////////////////////

    /// @brief The frequency of the wave, in decimal - ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    static const bool RainbowBeatAnimationEnabled = true;
    static const bool RainbowMarchAnimationEnabled = true;
    static const uint16_t RainbowAnimationSpeed = 10;
    /// @brief How many hue values to advance for each LED
    static const uint8_t RainbowAnimationHueDelta = 5;

    //////////////////////////////////////////
    // ANIMATIONS - PULSE
    //////////////////////////////////////////

    //// @brief The frequency of the wave, in decimal - ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    static const bool PulseAnimationEnabled = false;
    static const uint16_t PulseAnimationSpeed = 10;
    static const uint8_t PulseAnimationMinBrightness = 30;
    static const uint8_t PulseAnimationMaxBrightness = 255;


    //////////////////////////////////////////
    // TASKS
    //////////////////////////////////////////

    static const TickType_t TaskDelay = 1 / portTICK_PERIOD_MS;
    static const BaseType_t AnimationControllerTaskCore = 1;
    static const BaseType_t WiFiServiceTaskCore = 0;
    static const BaseType_t OTAServiceTaskCore = 0;
    static const BaseType_t MQTTClientTaskCore = 0;
    static const UBaseType_t AnimationControllerTaskPriority = 1;
    static const UBaseType_t WiFiServiceTaskPriority = 3;
    static const UBaseType_t OTAServiceTaskPriority = 2;
    static const UBaseType_t MQTTClientTaskPriority = 1;
    static const uint32_t AnimationControllerTaskStackSize = 15000;
    static const uint32_t WiFiServiceTaskStackSize = 4000;
    static const uint32_t OTAServiceTaskStackSize = 4000;
    static const uint32_t MQTTClientTaskStackSize = 4000;
}

#endif