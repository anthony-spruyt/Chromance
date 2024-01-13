/*
   A dot animation that travels along rails
   (C) Voidstar Lab LLC 2021
*/

#ifndef RIPPLE_H_
#define RIPPLE_H_

#include "map.h"
#include <FastLED.h>

namespace Chromance
{
    enum RippleState
    {
        // Ripple is to be deleted and should not lit up
        RIPPLE_STATE_DEAD,
        // Ripple isn't drawn as it passes through a node to keep the speed consistent
        RIPPLE_STATE_WITHIN_NODE,
        // Ripple is moving upwards
        RIPPLE_STATE_TRAVEL_UP,
        // Ripple is moving downwards
        RIPPLE_STATE_TRAVEL_DOWN
    };

    enum RippleBehavior
    {
        // Stop at next node
        RIPPLE_BEHAVIOR_COUCH_POTATO,
        // Only go straight
        RIPPLE_BEHAVIOR_LAZY,
        // Go straight if possible
        RIPPLE_BEHAVIOR_WEAK,
        RIPPLE_BEHAVIOR_FEISTY,
        RIPPLE_BEHAVIOR_ANGRY,
        RIPPLE_BEHAVIOR_ALWAYS_RIGHT,
        RIPPLE_BEHAVIOR_ALWAYS_LEFT,
        RIPPLE_BEHAVIOR_EXPLODING
    };

    class Ripple
    {
        public:

            Ripple();

            void Start(int32_t node, int32_t direction, CRGB color, float speed, unsigned long lifespan, RippleBehavior behavior);
            void Advance(CRGB* leds);
            void Claim(int32_t animationId);
            RippleState GetState();
            int32_t GetAnimationId();
            unsigned long GetStartedAt();
        
        private:

            void Render(CRGB* leds, unsigned long age);

            RippleState state;
            CRGB color;
            // The node if within a node otherwise the segment if travelling
            int32_t node;
            // The direction if within a node otherwise the LED postition from the bottom if travelling
            int32_t direction;
            // Each loop the ripples move this many LED's
            float speed;
            // The ripple stops after this many milliseconds
            unsigned long lifespan;
            RippleBehavior behavior;
            bool justStarted;
            // The ripple will move When pressure reaches 1
            float pressure;
            // Used to track age of ripple
            unsigned long startedAt;
            // The ID of the animation that has claimed this ripple
            int32_t animationId;
        };
}

#endif
