#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <Arduino.h>

namespace Chromance
{
    //////////////////////////////////////////
    // COMMON STRINGS
    //////////////////////////////////////////
    constexpr const char* ChromanceNameCapitalized = "Chromance";
    constexpr const char* ChromanceNameLowercase = "chromance";

    //////////////////////////////////////////
    // WiFi
    //////////////////////////////////////////

    // The connection timeout in seconds before reboot
    constexpr uint32_t WifiConnectionTimeout = 15U;

    //////////////////////////////////////////
    // OTA
    //////////////////////////////////////////

    // HTTP firmware upload endpoint, authenticated with OTAHttpUsername and OTAPassword
    constexpr uint16_t OTAHttpPort = 80U;
    constexpr const char* OTAHttpPath = "/update";
    constexpr const char* OTAHttpUsername = "chromance";

    //////////////////////////////////////////
    // TIME
    //////////////////////////////////////////

    // The timeout in seconds for syncing internet time
    constexpr uint32_t NTPSyncTimeout = 10U;
    // Local timezone
    constexpr const char* TimeZoneLocation = "Australia/Melbourne";

    //////////////////////////////////////////
    // MQTT
    //////////////////////////////////////////

    constexpr uint16_t MQTTKeepAlive = 60U;
    constexpr const char* MQTTBaseTopic = "chromance/v1";
    constexpr const char* MQTTCommandRoute = "~/command";
    constexpr const char* MQTTCommandTopic = "chromance/v1/command";
    constexpr const char* MQTTStateRoute = "~/state";
    constexpr const char* MQTTStateTopic = "chromance/v1/state";
    constexpr const char* HomeAssistantStatusTopic = "homeassistant/status";
    constexpr int32_t PublishJsonBufferSize = 2048;
    constexpr bool ChromanceStateUpdatesEnabled = true;
    constexpr unsigned long ChromanceStateUpdateFrequency = 5000UL;
    constexpr unsigned long ChromanceSleepingStateUpdateFrequency = 15000UL;

    //////////////////////////////////////////
    // CONFIG
    //////////////////////////////////////////

    constexpr const char* ConfigNamespace = "config";
    constexpr const char* LogLevelConfigKey = "ll";
    constexpr const char* BrightnessConfigKey = "l";
    constexpr const char* SleepingConfigKey = "s";
    constexpr const char* AnimationSpeedConfigKeyPrefix = "as";
    constexpr const char* RippleLifespanConfigKeyPrefix = "rl";
    constexpr const char* RipplePulsePeriodConfigKeyPrefix = "rp";
    constexpr const char* RippleDecayConfigKeyPrefix = "rd";

    //////////////////////////////////////////
    // LEDs
    //////////////////////////////////////////

    constexpr uint32_t BlueStripDataPin = 33U;
    constexpr uint32_t GreenStripDataPin = 27U;
    constexpr uint32_t RedStripDataPin = 2U;
    constexpr uint32_t BlackStripDataPin = 4U;
    constexpr uint32_t BlueStripOffset = 0U;
    constexpr uint32_t BlueStripLength = 154U;
    constexpr uint32_t GreenStripOffset = BlueStripLength;
    constexpr uint32_t GreenStripLength = 168U;
    constexpr uint32_t RedStripOffset = GreenStripLength + BlueStripLength;
    constexpr uint32_t RedStripLength = 84U;
    constexpr uint32_t BlackStripOffset = RedStripLength + GreenStripLength + BlueStripLength;
    constexpr uint32_t BlackStripLength = 154U;
    constexpr uint32_t NumberOfLEDs = BlueStripLength + GreenStripLength + RedStripLength + BlackStripLength;
    constexpr uint32_t MaxRefreshRate = 120U;
    constexpr uint8_t StartupBrightness = 1U;
    constexpr uint32_t StartupDelay = 500U;
    constexpr uint32_t BlueStripIndex = 0U;
    constexpr uint32_t GreenStripIndex = 1U;
    constexpr uint32_t RedStripIndex = 2U;
    constexpr uint32_t BlackStripIndex = 3U;
    constexpr uint32_t MaxPathsPerNode = 6U;
    constexpr uint32_t LEDsPerSegment = 14U;

    //////////////////////////////////////////
    // ANIMATIONS
    //////////////////////////////////////////

    // The duration in milliseconds when in random animation mode that each animation is played before transitioning to the next randomly selected animation
    constexpr unsigned long RandomAnimationDuration = 30000UL;
    // The duration in milliseconds of a fade between animations, or into and out of sleep
    constexpr unsigned long AnimationTransitionDuration = 1500UL;

    //////////////////////////////////////////
    // ANIMATIONS - RAINBOW
    //////////////////////////////////////////

    constexpr bool RainbowBeatAnimationEnabled = true;
    constexpr bool RainbowMarchAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    constexpr uint16_t RainbowBeatAnimationSpeed = 10U;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    constexpr uint16_t RainbowMarchAnimationSpeed = 10U;
    // How many hue values to advance for each LED
    constexpr uint8_t RainbowBeatAnimationHueDelta = 5U;
    // How many hue values to advance for each LED
    constexpr uint8_t RainbowMarchAnimationHueDelta = 5U;

    //////////////////////////////////////////
    // ANIMATIONS - PULSE
    //////////////////////////////////////////

    constexpr bool PulseAnimationEnabled = true;
    /**
     * The frequency of the wave, in decimal
     * ANSI: unsigned short _Accum. 8 bits int, 8 bits fraction
    */
    constexpr uint16_t PulseAnimationSpeed = 10U;
    constexpr uint8_t PulseAnimationMinBrightness = 30U;
    constexpr uint8_t PulseAnimationMaxBrightness = UINT8_MAX;
    constexpr uint8_t PulseAnimationNumberOfColors = 7U;
    // Hex color codes
    constexpr uint32_t PulseAnimationColors[PulseAnimationNumberOfColors] =
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

    constexpr bool CubePulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - STAR BURST PULSE
    //////////////////////////////////////////

    constexpr bool StarBurstPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - CENTER PULSE
    //////////////////////////////////////////

    constexpr bool CenterPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - CENTER PULSE
    //////////////////////////////////////////

    constexpr bool RandomPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - AROUND THE WORLD
    //////////////////////////////////////////

    constexpr bool AroundTheWorldAnimationEnabled = true;

    //////////////////////////////////////////
    // TASKS
    //////////////////////////////////////////

    constexpr TickType_t TaskDelay = 1 / portTICK_PERIOD_MS;
    constexpr BaseType_t AnimationControllerTaskCore = 1;
    constexpr BaseType_t WiFiServiceTaskCore = 0;
    constexpr BaseType_t OTAServiceTaskCore = 0;
    constexpr BaseType_t MQTTClientTaskCore = 0;
    constexpr UBaseType_t AnimationControllerTaskPriority = 1U;
    constexpr UBaseType_t WiFiServiceTaskPriority = 3U;
    constexpr UBaseType_t OTAServiceTaskPriority = 2U;
    constexpr UBaseType_t MQTTClientTaskPriority = 1U;
    constexpr uint32_t AnimationControllerTaskStackSize = 8000U;
    constexpr uint32_t WiFiServiceTaskStackSize = 4000U;
    constexpr uint32_t OTAServiceTaskStackSize = 8000U;
    constexpr uint32_t MQTTClientTaskStackSize = 4000U;
}

#endif
