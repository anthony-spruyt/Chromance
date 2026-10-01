#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../globals.h"
#include "../services/logger.h"
#include <FastLED.h>

namespace Chromance
{
    class Animation
    {
        public:

            Animation(int32_t id, const char* name, Config* config, Logger* logger);
            virtual ~Animation() = default;

            virtual void Loop() = 0;
            virtual void Sleep(bool fade);
            virtual void Wake(bool fade);
            uint8_t GetID();
            AnimationType GetAnimationType();
            const char* GetName();
            CRGB* GetBuffer();
            AnimationStatus GetStatus();
            uint8_t GetTransitionScale();
            void Transition();
            virtual bool IsRippleAnimation();

        protected:

            virtual void Reset();
            float GetSpeed();
            float GetParameter(uint8_t index);
            // Seconds since the last call, scaled by the speed setting. Call it once per Loop()
            float GetElapsedSeconds();

            int32_t id;
            const char* name;
            Config* config;
            Logger* logger;
            CRGB leds[NumberOfLEDs];
            uint8_t transitionScale;
            uint8_t transitionStartScale;
            unsigned long transitionStartedAt;
            AnimationStatus status;

        private:

            unsigned long lastElapsedAt;
    };
}

#endif
