#ifndef CONFIG_H_
#define CONFIG_H_

#include "../globals.h"
#include <Preferences.h>

namespace Chromance
{
    class Config
    {
        public:

            Config();
            ~Config();

            void Setup();
            void SetLogLevel(uint8_t value);
            uint8_t GetLogLevel();
            void SetBrightness(uint8_t value);
            uint8_t GetBrightness();
            void SetSleeping(bool value);
            bool GetSleeping();
            void SetTransitionDuration(unsigned long value);
            unsigned long GetTransitionDuration();
            void SetRippleStepsPerSecond(uint32_t value);
            uint32_t GetRippleStepsPerSecond();
            void SetMaxBrightness(uint8_t value);
            uint8_t GetMaxBrightness();
            void SetMaxCurrent(uint32_t value);
            uint32_t GetMaxCurrent();

            float GetAnimationSpeed(AnimationType animationType);
            unsigned long GetRippleLifespan(AnimationType animationType);
            unsigned long GetRipplePulsePeriod(AnimationType animationType);
            uint8_t GetRippleDecay(AnimationType animationType);

            void SetAnimationSpeed(AnimationType animationType, float value);
            void SetRippleLifespan(AnimationType animationType, unsigned long value);
            void SetRipplePulsePeriod(AnimationType animationType, unsigned long value);
            void SetRippleDecay(AnimationType animationType, uint8_t value);

            String GetAnimationSpeedKey(AnimationType animationType);
            String GetRippleLifespanKey(AnimationType animationType);
            String GetRipplePulsePeriodKey(AnimationType animationType);
            String GetRippleDecayKey(AnimationType animationType);

        private:

            Preferences preferences;
            SemaphoreHandle_t semaphore;
            uint8_t logLevel;
            uint8_t brightness;
            bool sleeping;
            unsigned long transitionDuration;
            uint32_t rippleStepsPerSecond;
            uint8_t maxBrightness;
            uint32_t maxCurrent;
            float animationSpeed[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long rippleLifespan[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long ripplePulsePeriod[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            uint8_t rippleDecay[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
    };
}

#endif
