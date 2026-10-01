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
            /**
             * Write changed values to NVS once they have been unchanged for ConfigSaveDelay.
             * Flash writes stall the LED driver, so call this only between frames
             * @param now Write immediately, e.g. before a reboot
             */
            void Save(bool now = false);
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
            void SetRandomAnimationDuration(uint32_t value);
            uint32_t GetRandomAnimationDuration();

            float GetAnimationSpeed(AnimationType animationType);
            unsigned long GetRippleLifespan(AnimationType animationType);
            unsigned long GetRipplePulsePeriod(AnimationType animationType);
            uint8_t GetRippleDecay(AnimationType animationType);

            void SetAnimationSpeed(AnimationType animationType, float value);
            void SetRippleLifespan(AnimationType animationType, unsigned long value);
            void SetRipplePulsePeriod(AnimationType animationType, unsigned long value);
            void SetRippleDecay(AnimationType animationType, uint8_t value);
            float GetAnimationParameter(AnimationType animationType, uint8_t index);
            // Clamped to the parameter's min and max
            void SetAnimationParameter(AnimationType animationType, uint8_t index, float value);

            String GetAnimationSpeedKey(AnimationType animationType);
            String GetRippleLifespanKey(AnimationType animationType);
            String GetRipplePulsePeriodKey(AnimationType animationType);
            String GetRippleDecayKey(AnimationType animationType);
            String GetAnimationParameterKey(AnimationType animationType, uint8_t index);

        private:

            void SaveUShort(const char* key, uint16_t value);
            void SaveULong(const char* key, uint32_t value);
            void SaveFloat(const char* key, float value);
            void SaveBool(const char* key, bool value);

            Preferences preferences;
            SemaphoreHandle_t semaphore;
            uint8_t logLevel;
            uint8_t brightness;
            bool sleeping;
            unsigned long transitionDuration;
            uint32_t rippleStepsPerSecond;
            uint8_t maxBrightness;
            uint32_t maxCurrent;
            uint32_t randomAnimationDuration;
            bool dirty;
            unsigned long changedAt;
            float animationSpeed[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long rippleLifespan[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long ripplePulsePeriod[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            uint8_t rippleDecay[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            float animationParameters[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS][MaxAnimationParameters];
    };
}

#endif
