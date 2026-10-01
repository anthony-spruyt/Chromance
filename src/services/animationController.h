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
            // Estimated LED current draw in milliamps
            uint32_t GetCurrent();
            Animation* GetAnimation(AnimationType animationType);

        private:

            void HandleAnimationRequest();
            void HandleRandomAnimation();
            // Wakes an animation and puts every other animation to sleep
            void Show(AnimationType animationType);
            void Render();
            uint8_t GetFadedBrightness();
            AnimationType NextAnimation();

            Logger* logger;
            Config* config;
            CRGB leds[NumberOfLEDs];
            Animation* animations[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS];
            AnimationType currentAnimationType;
            SemaphoreHandle_t semaphore;
            unsigned long lastRandomAnimationStarted;
            AnimationRequest next;
            RipplePool ripplePool;
            uint8_t brightness;
            uint8_t brightnessFrom;
            uint8_t brightnessTarget;
            unsigned long brightnessChangedAt;
            uint32_t current;
    };
}

#endif
