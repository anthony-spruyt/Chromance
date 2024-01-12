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
    enum AnimationType
    {
        ANIMATION_TYPE_RANDOM_ANIMATION, // DO NOT MOVE- We have to use auto assignment and this needs to be 0
        ANIMATION_TYPE_STRIP_TEST, // DO NOT MOVE- We have to use auto assignment and this needs to be 1
        ANIMATION_TYPE_RANDOM_PULSE,
        ANIMATION_TYPE_CUBE_PULSE,
        ANIMATION_TYPE_STAR_BURST_PULSE,
        ANIMATION_TYPE_CENTER_PULSE,
        ANIMATION_TYPE_RAINBOW_BEAT,
        ANIMATION_TYPE_RAINBOW_MARCH,
        ANIMATION_TYPE_PULSE,
        /**
         * This is not an animation type. It is a hack to get the count of animation types
         * DO NOT MOVE- We have to use auto assignment and this needs to be the last
        */
        ANIMATION_TYPE_NUMBER_OF_ANIMATIONS
    };

    enum AnimationRequest
    {
        ANIMATION_REQUEST_NONE,
        ANIMATION_REQUEST_PLAY,
        ANIMATION_REQUEST_SLEEP,
        ANIMATION_REQUEST_WAKE
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
            Animation* animations[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
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