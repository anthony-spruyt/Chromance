#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../globals.h"
#include <FastLED.h>

namespace Chromance
{
    enum class AnimationStatus : uint8_t
    {
        Playing,
        Sleeping,
        WakingUp,
        GoingToSleep
    };

    class Animation
    {
        public:

            Animation(uint8_t id, const char* name);

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

            uint8_t id;
            CRGB leds[NumberOfLEDs];
            const char* name;
            uint8_t transitionScale;
            AnimationStatus status;
            bool fade;
            
    };
}

#endif