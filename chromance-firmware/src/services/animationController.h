#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_

#include "../globals.h"
#include "logger.h"
#include "config.h"
#include <FastLED.h>
#include "../animations/animation.h"
#include "../animations/chromance/ripplePool.h"

namespace Chromance
{
    enum class AnimationType : uint8_t
    {
        RandomAnimation, // DO NOT MOVE- We have to use auto assignment and this needs to be 0
        StripTest, // DO NOT MOVE- We have to use auto assignment and this needs to be 1
        RandomPulse,
        CubePulse,
        StarburstPulse,
        CenterPulse,
        RainbowBeat,
        RainbowMarch,
        Pulse,
        /**
         * This is not an animation type. It is a hack to get the count of animation types
         * DO NOT MOVE- We have to use auto assignment and this needs to be the last
        */
        NumberOfAnimations
    };

    enum class AnimationRequest : uint8_t
    {
        None,
        Play,
        Sleep,
        Wake
    };

    class AnimationController
    {
        public:
        
            AnimationController(Logger* logger, Config* config);
            ~AnimationController();

            void Setup();
            void Loop();
            // Puts the LED display to sleep
            void Sleep();
            // Wakes the LED display from sleep
            void Wake();
            /**
             * Start a new type of animation
             * @param animationType The type of animation to start
            */
            void Play(AnimationType animationType);

        private:

            AnimationType RandomAnimation();

            Logger* logger;
            Config* config;
            CRGB leds[NumberOfLEDs];
            Animation* animations[(uint8_t)AnimationType::NumberOfAnimations];
            AnimationType currentAnimationType;
            SemaphoreHandle_t semaphore;
            bool sleeping;
            unsigned long lastRandomAnimationStarted;
            uint8_t transitionScale;
            AnimationRequest next;
            RipplePool ripplePool;
    };
}

#endif