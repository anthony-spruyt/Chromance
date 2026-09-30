#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_

#include "../globals.h"
#include "logger.h"
#include "config.h"
#include <FastLED.h>
#include "../animations/animation.h"
#include "../animations/ripples/ripplePool.h"

namespace Chromance
{
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
            AnimationType GetAnimationType();
            AnimationStatus GetAnimationStatus();
            uint8_t GetBrightness();
            void SetBrightness(uint8_t value);
            uint32_t GetFPS();
            Animation* GetAnimation(AnimationType animationType);

        private:

            void HandleBrightness();
            void HandleAnimationRequest();
            void HandleRandomAnimation();
            void Render();
            AnimationType NextAnimation();

            Logger* logger;
            Config* config;
            CRGB leds[NumberOfLEDs];
            Animation* animations[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            AnimationType currentAnimationType;
            SemaphoreHandle_t semaphore;
            unsigned long lastRandomAnimationStarted;
            uint8_t transitionScale;
            AnimationRequest next;
            RipplePool ripplePool;
    };
}

#endif