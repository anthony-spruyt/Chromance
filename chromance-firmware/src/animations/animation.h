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

            virtual void Loop() = 0;
            virtual void Sleep(bool fade);
            virtual void Wake(bool fade);
            uint8_t GetID();
            AnimationType GetAnimationType();
            const char* GetName();
            CRGB* GetBuffer();
            AnimationStatus GetStatus();
            virtual void Transition();
            virtual bool IsRippleAnimation();

        protected:

            virtual void Fade();
            float GetSpeed();

            int32_t id;
            const char* name;
            Config* config;
            Logger* logger;
            CRGB leds[NumberOfLEDs];
            uint8_t transitionScale;
            AnimationStatus status;
            bool fade;
    };
}

#endif