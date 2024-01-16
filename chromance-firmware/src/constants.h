#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <Arduino.h>

namespace Chromance
{
    //////////////////////////////////////////
    // COMMON STRINGS
    //////////////////////////////////////////
    static const String ChromanceNameCapitalized = "Chromance";
    static const String ChromanceNameLowercase = "chromance";

    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    // The connection timeout in seconds before reboot
    static const uint32_t WifiConnectionTimeout = 15;

    //////////////////////////////////////////
    // TIME
    //////////////////////////////////////////

    // The timeout in seconds for syncing internet time
    static const uint32_t NTPSyncTimeout = 10;
    // Local timezone
    static const String TimeZoneLocation = "Australia/Melbourne";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    static const uint16_t MQTTKeepAlive = 60;
    static const String MQTTBaseTopic = "chromance/v1";
    static const String MQTTCommandRoute = "~/command";
    static const String MQTTCommandTopic = "chromance/v1/command";
    static const String MQTTStateRoute = "~/state";
    static const String MQTTStateTopic = "chromance/v1/state";
    static const String HomeAssistantStatusTopic = "homeassistant/status";
    static const int32_t PublishJSONBufferSize = 1024;
    static const bool ChromanceStateUpdatesEnabled = true;
    static const unsigned long ChromanceStateUpdateFrequency = 5000U;

    //////////////////////////////////////////
    // CONFIG
    //////////////////////////////////////////

    // The preferences namespace
    static const char* ConfigNamespace = "config";
    // The preferences log level key
    static const char* LogLevelConfigKey = "ll";
    // They preferences brightness key
    static const char* BrightnessConfigKey = "l";
    static const char* SleepingConfigKey = "s";

    //////////////////////////////////////////
    // LEDs
    //////////////////////////////////////////

    static const uint32_t BlueStripDataPin = 33;
    static const uint32_t GreenStripDataPin = 27;
    static const uint32_t RedStripDataPin = 2;
    static const uint32_t BlackStripDataPin = 4;
    static const uint32_t BlueStripOffset = 0;
    static const uint32_t BlueStripLength = 154;
    static const uint32_t GreenStripOffset = BlueStripLength;
    static const uint32_t GreenStripLength = 168;
    static const uint32_t RedStripOffset = GreenStripLength + BlueStripLength;
    static const uint32_t RedStripLength = 84;
    static const uint32_t BlackStripOffset = RedStripLength + GreenStripLength + BlueStripLength;
    static const uint32_t BlackStripLength = 154;
    static const uint32_t NumberOfLEDs = BlueStripLength + GreenStripLength + RedStripLength + BlackStripLength;
    static const uint32_t MaxRefreshRate = 30;
    static const uint8_t StartupBrightness = 1;
    static const uint64_t StartupDelay = 5000;
    static const uint32_t BlueStripIndex = 0;
    static const uint32_t GreenStripIndex = 1;
    static const uint32_t RedStripIndex = 2;
    static const uint32_t BlackStripIndex = 3;
    static const uint32_t MaxPathsPerNode = 6;
    static const uint32_t LEDsPerSegment = 14;

    //////////////////////////////////////////
    // ANIMATIONS
    //////////////////////////////////////////

    // The duration in milliseconds when in random animation mode that each animation is played before transitioning to the next randomly selected animation
    static const unsigned long RandomAnimationDuration = 30000;
    static const uint8_t AnimationTransitionSpeed = 20;

    //////////////////////////////////////////
    // ANIMATIONS - RAINBOW
    //////////////////////////////////////////

    static const bool RainbowBeatAnimationEnabled = true;
    static const bool RainbowMarchAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    static const uint16_t RainbowAnimationSpeed = 10;
    // How many hue values to advance for each LED
    static const uint8_t RainbowAnimationHueDelta = 5;

    //////////////////////////////////////////
    // ANIMATIONS - PULSE
    //////////////////////////////////////////

    static const bool PulseAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    static const uint16_t PulseAnimationSpeed = 10;
    static const uint8_t PulseAnimationMinBrightness = 30;
    static const uint8_t PulseAnimationMaxBrightness = 255;
    static const uint8_t PulseAnimationNumberOfColors = 7;
    // Hex color codes
    static const uint32_t PulseAnimationColors[PulseAnimationNumberOfColors] =
    {
        0x006400, // DarkGreen
        0x8B0000, // DarkRed
        0x9400D3, // DarkViolet
        0x00008B, // DarkBlue
        0x8B008B, // DarkMagenta
        0x9400D3, // DarkViolet
        0x00CED1 // DarkTurquoise
    };

    //////////////////////////////////////////
    // ANIMATIONS - CUBE PULSE
    //////////////////////////////////////////

    static const bool CubePulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - STAR BURST PULSE
    //////////////////////////////////////////

    static const bool StarBurstPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - CENTER PULSE
    //////////////////////////////////////////

    static const bool CenterPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - CENTER PULSE
    //////////////////////////////////////////

    static const bool RandomPulseAnimationEnabled = true;

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
    static const uint32_t AnimationControllerTaskStackSize = 8000;
    static const uint32_t WiFiServiceTaskStackSize = 4000;
    static const uint32_t OTAServiceTaskStackSize = 4000;
    static const uint32_t MQTTClientTaskStackSize = 4000;
}

#endif