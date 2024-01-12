#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../globals.h"
#include "../services/logger.h"
#include <FastLED.h>

namespace Chromance
{
    enum AnimationStatus
    {
        ANIMATION_STATUS_PLAYING,
        ANIMATION_STATUS_SLEEPING,
        ANIMATION_STATUS_WAKING_UP,
        ANIMATION_STATUS_GOING_TO_SLEEP
    };

    class Animation
    {
        public:

            Animation(int32_t id, const char* name, Logger* logger);

            virtual void Loop() = 0;
            virtual void Sleep(bool fade);
            virtual void Wake(bool fade);
            uint8_t GetID();
            const char* GetName();
            CRGB* GetBuffer();
            AnimationStatus GetStatus();
            virtual void Transition();

        protected:

            virtual void Fade();

            int32_t id;
            const char* name;
            Logger* logger;
            CRGB leds[NumberOfLEDs];
            uint8_t transitionScale;
            AnimationStatus status;
            bool fade;
            
    };
}

#endif