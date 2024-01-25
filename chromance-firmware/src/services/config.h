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

            void Setup();
            void SetLogLevel(uint8_t value);
            uint8_t GetLogLevel();
            void SetBrightness(uint8_t value);
            uint8_t GetBrightness();
            void SetSleeping(bool value);
            bool GetSleeping();
            
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
            uint8_t logLevel;
            uint8_t brightness;
            bool sleeping;
            float animationSpeed[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long rippleLifespan[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            unsigned long ripplePulsePeriod[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            uint8_t rippleDecay[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];

    };
}

#endif