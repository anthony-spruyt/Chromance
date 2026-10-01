#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <Arduino.h>
#include "models.h"

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
    // Changes are written to flash after this many milliseconds without another change, so dragging a slider writes once
    constexpr unsigned long ConfigSaveDelay = 10000UL;
    constexpr const char* LogLevelConfigKey = "ll";
    constexpr const char* BrightnessConfigKey = "l";
    constexpr const char* SleepingConfigKey = "s";
    constexpr const char* AnimationSpeedConfigKeyPrefix = "as";
    constexpr const char* RippleLifespanConfigKeyPrefix = "rl";
    constexpr const char* RipplePulsePeriodConfigKeyPrefix = "rp";
    constexpr const char* RippleDecayConfigKeyPrefix = "rd";
    constexpr const char* TransitionDurationConfigKey = "td";
    constexpr const char* RippleStepsPerSecondConfigKey = "rs";
    constexpr const char* MaxBrightnessConfigKey = "mb";
    constexpr const char* MaxCurrentConfigKey = "mc";
    constexpr const char* RandomAnimationDurationConfigKey = "ra";
    // Followed by the animation number, an underscore and the parameter number, e.g. ap14_0
    constexpr const char* AnimationParameterConfigKeyPrefix = "ap";

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
    // Percent of full brightness that 100% in Home Assistant maps to
    constexpr uint8_t DefaultMaxBrightness = 66U;
    constexpr uint32_t LEDVoltage = 5U;
    // FastLED assumes ~42mA per white LED but real ones can draw ~60mA, so the default leaves headroom under the 30A supply
    constexpr uint32_t DefaultMaxCurrent = 20000U;
    constexpr uint32_t MinMaxCurrent = 1000U;
    constexpr uint32_t MaxMaxCurrent = 30000U;
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

    // The duration in seconds when in random animation mode that each animation is played before transitioning to the next randomly selected animation
    constexpr uint32_t DefaultRandomAnimationDuration = 30U;
    constexpr uint32_t MinRandomAnimationDuration = 5U;
    constexpr uint32_t MaxRandomAnimationDuration = 3600U;
    // The duration in milliseconds of a fade between animations, or into and out of sleep
    constexpr unsigned long DefaultTransitionDuration = 1500UL;
    constexpr unsigned long MaxTransitionDuration = 10000UL;
    constexpr uint8_t MaxAnimationParameters = 4U;

    template <size_t N>
    constexpr uint8_t NumberOfParameters(const AnimationParameter (&)[N])
    {
        static_assert(N <= MaxAnimationParameters, "Raise MaxAnimationParameters");

        return N;
    }

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
    // ANIMATIONS - RANDOM PULSE
    //////////////////////////////////////////

    constexpr bool RandomPulseAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - AROUND THE WORLD
    //////////////////////////////////////////

    constexpr bool AroundTheWorldAnimationEnabled = true;

    //////////////////////////////////////////
    // ANIMATIONS - PARAMETERS
    // Each animation's sliders in Home Assistant: {name, min, max, step, default, unit}
    // A parameter's position is its config key and Home Assistant ID, so only ever append, and keep each enum in the same order
    //////////////////////////////////////////

    constexpr bool PlasmaAnimationEnabled = true;
    enum PlasmaParameter { PLASMA_DETAIL, PLASMA_MORPH_SPEED, PLASMA_COLOR_SPEED };
    constexpr AnimationParameter PlasmaAnimationParameters[] =
    {
        {"Detail", 1.0f, 20.0f, 1.0f, 6.0f, nullptr},
        {"Morph Speed", 0.0f, 1000.0f, 5.0f, 125.0f, nullptr},
        {"Color Speed", 0.0f, 255.0f, 1.0f, 10.0f, nullptr}
    };

    constexpr bool RadarAnimationEnabled = true;
    enum RadarParameter { RADAR_TURNS_PER_MINUTE, RADAR_TRAIL, RADAR_COLOR_SPEED };
    constexpr AnimationParameter RadarAnimationParameters[] =
    {
        {"Turns Per Minute", 1.0f, 120.0f, 1.0f, 20.0f, nullptr},
        {"Trail", 1.0f, 255.0f, 1.0f, 96.0f, nullptr},
        {"Color Speed", 0.0f, 255.0f, 1.0f, 10.0f, nullptr}
    };

    constexpr bool RainbowSwirlAnimationEnabled = true;
    enum RainbowSwirlParameter { RAINBOW_SWIRL_SPIN_SPEED, RAINBOW_SWIRL_TWIST };
    constexpr AnimationParameter RainbowSwirlAnimationParameters[] =
    {
        {"Spin Speed", 0.0f, 500.0f, 2.0f, 100.0f, nullptr},
        {"Twist", -254.0f, 254.0f, 2.0f, 128.0f, nullptr}
    };

    constexpr bool RingsAnimationEnabled = true;
    enum RingsParameter { RINGS_COUNT, RINGS_RING_SPEED, RINGS_COLOR_SPEED };
    constexpr AnimationParameter RingsAnimationParameters[] =
    {
        {"Count", 1.0f, 10.0f, 1.0f, 3.0f, nullptr},
        {"Ring Speed", 0.0f, 1000.0f, 5.0f, 250.0f, nullptr},
        {"Color Speed", 0.0f, 255.0f, 1.0f, 20.0f, nullptr}
    };

    constexpr bool FireAnimationEnabled = true;
    enum FireParameter { FIRE_RISE_SPEED, FIRE_FLICKER_SPEED, FIRE_COOLING, FIRE_DETAIL };
    constexpr AnimationParameter FireAnimationParameters[] =
    {
        {"Rise Speed", 0.0f, 1000.0f, 5.0f, 400.0f, nullptr},
        {"Flicker Speed", 0.0f, 1000.0f, 5.0f, 120.0f, nullptr},
        {"Cooling", 0.0f, 255.0f, 1.0f, 160.0f, nullptr},
        {"Detail", 1.0f, 20.0f, 1.0f, 6.0f, nullptr}
    };
    // HeatColors_p blends back to black past this index
    constexpr uint8_t FireAnimationMaxPaletteIndex = 240U;

    constexpr bool TwinkleAnimationEnabled = true;
    enum TwinkleParameter { TWINKLE_PER_SECOND, TWINKLE_FADE_TIME };
    constexpr AnimationParameter TwinkleAnimationParameters[] =
    {
        {"Twinkles Per Second", 0.5f, 50.0f, 0.5f, 10.0f, nullptr},
        {"Fade Time", 100.0f, 10000.0f, 50.0f, 1500.0f, "ms"}
    };

    inline const AnimationParameter* GetAnimationParameters(AnimationType animationType, uint8_t& count)
    {
        switch (animationType)
        {
            case ANIMATION_TYPE_PLASMA:
                count = NumberOfParameters(PlasmaAnimationParameters);
                return PlasmaAnimationParameters;
            case ANIMATION_TYPE_RADAR:
                count = NumberOfParameters(RadarAnimationParameters);
                return RadarAnimationParameters;
            case ANIMATION_TYPE_RAINBOW_SWIRL:
                count = NumberOfParameters(RainbowSwirlAnimationParameters);
                return RainbowSwirlAnimationParameters;
            case ANIMATION_TYPE_RINGS:
                count = NumberOfParameters(RingsAnimationParameters);
                return RingsAnimationParameters;
            case ANIMATION_TYPE_FIRE:
                count = NumberOfParameters(FireAnimationParameters);
                return FireAnimationParameters;
            case ANIMATION_TYPE_TWINKLE:
                count = NumberOfParameters(TwinkleAnimationParameters);
                return TwinkleAnimationParameters;
            default:
                count = 0U;
                return nullptr;
        }
    }

    //////////////////////////////////////////
    // ANIMATIONS - RIPPLES
    //////////////////////////////////////////

    // Ripple speeds and trail decay were tuned at this rate, so changing it changes how every ripple animation looks
    constexpr uint32_t DefaultRippleStepsPerSecond = 60U;
    constexpr uint32_t MaxRippleStepsPerSecond = 240U;
    // The largest lifespan Home Assistant offers, in milliseconds
    constexpr unsigned long RippleMaxLifespan = 30000UL;

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
