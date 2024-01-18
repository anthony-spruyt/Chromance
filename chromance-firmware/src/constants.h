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
    static const uint32_t WifiConnectionTimeout = 15U;

    //////////////////////////////////////////
    // TIME
    //////////////////////////////////////////

    // The timeout in seconds for syncing internet time
    static const uint32_t NTPSyncTimeout = 10U;
    // Local timezone
    static const String TimeZoneLocation = "Australia/Melbourne";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    static const uint16_t MQTTKeepAlive = 60U;
    static const String MQTTBaseTopic = "chromance/v1";
    static const String MQTTCommandRoute = "~/command";
    static const String MQTTCommandTopic = "chromance/v1/command";
    static const String MQTTStateRoute = "~/state";
    static const String MQTTStateTopic = "chromance/v1/state";
    static const String HomeAssistantStatusTopic = "homeassistant/status";
    static const int32_t PublishJSONBufferSize = 1024;
    static const bool ChromanceStateUpdatesEnabled = true;
    static const unsigned long ChromanceStateUpdateFrequency = 5000UL;
    static const unsigned long ChromanceSleepingStateUpdateFrequency = 60000UL;

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
    static const char* AnimationSpeedConfigKeyPrefix = "as";
    static const char* RippleLifespanConfigKeyPrefix = "rl";
    static const char* RipplePulsePeriodConfigKeyPrefix = "rp";
    static const char* RippleDecayConfigKeyPrefix = "rd";

    //////////////////////////////////////////
    // LEDs
    //////////////////////////////////////////

    static const uint32_t BlueStripDataPin = 33U;
    static const uint32_t GreenStripDataPin = 27U;
    static const uint32_t RedStripDataPin = 2U;
    static const uint32_t BlackStripDataPin = 4U;
    static const uint32_t BlueStripOffset = 0U;
    static const uint32_t BlueStripLength = 154U;
    static const uint32_t GreenStripOffset = BlueStripLength;
    static const uint32_t GreenStripLength = 168U;
    static const uint32_t RedStripOffset = GreenStripLength + BlueStripLength;
    static const uint32_t RedStripLength = 84U;
    static const uint32_t BlackStripOffset = RedStripLength + GreenStripLength + BlueStripLength;
    static const uint32_t BlackStripLength = 154U;
    static const uint32_t NumberOfLEDs = BlueStripLength + GreenStripLength + RedStripLength + BlackStripLength;
    static const uint32_t MaxRefreshRate = 120U;
    static const uint8_t StartupBrightness = 1U;
    static const uint32_t StartupDelay = 5000U;
    static const uint32_t BlueStripIndex = 0U;
    static const uint32_t GreenStripIndex = 1U;
    static const uint32_t RedStripIndex = 2U;
    static const uint32_t BlackStripIndex = 3U;
    static const uint32_t MaxPathsPerNode = 6U;
    static const uint32_t LEDsPerSegment = 14U;

    //////////////////////////////////////////
    // ANIMATIONS
    //////////////////////////////////////////

    // The duration in milliseconds when in random animation mode that each animation is played before transitioning to the next randomly selected animation
    static const unsigned long RandomAnimationDuration = 30000UL;
    static const uint8_t AnimationTransitionSpeed = 5U;

    //////////////////////////////////////////
    // ANIMATIONS - RAINBOW
    //////////////////////////////////////////

    static const bool RainbowBeatAnimationEnabled = true;
    static const bool RainbowMarchAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    static const uint16_t RainbowAnimationSpeed = 10U;
    // How many hue values to advance for each LED
    static const uint8_t RainbowAnimationHueDelta = 5U;

    //////////////////////////////////////////
    // ANIMATIONS - PULSE
    //////////////////////////////////////////

    static const bool PulseAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    static const uint16_t PulseAnimationSpeed = 10U;
    static const uint8_t PulseAnimationMinBrightness = 30U;
    static const uint8_t PulseAnimationMaxBrightness = 255U;
    static const uint8_t PulseAnimationNumberOfColors = 7U;
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
    // ANIMATIONS - AROUND THE WORLD
    //////////////////////////////////////////

    static const bool AroundTheWorldAnimationEnabled = true;

    //////////////////////////////////////////
    // TASKS
    //////////////////////////////////////////

    static const TickType_t TaskDelay = 1 / portTICK_PERIOD_MS;
    static const BaseType_t AnimationControllerTaskCore = 1;
    static const BaseType_t WiFiServiceTaskCore = 0;
    static const BaseType_t OTAServiceTaskCore = 0;
    static const BaseType_t MQTTClientTaskCore = 0;
    static const UBaseType_t AnimationControllerTaskPriority = 1U;
    static const UBaseType_t WiFiServiceTaskPriority = 3U;
    static const UBaseType_t OTAServiceTaskPriority = 2U;
    static const UBaseType_t MQTTClientTaskPriority = 1U;
    static const uint32_t AnimationControllerTaskStackSize = 8000U;
    static const uint32_t WiFiServiceTaskStackSize = 4000U;
    static const uint32_t OTAServiceTaskStackSize = 4000U;
    static const uint32_t MQTTClientTaskStackSize = 4000U;
}

#endif