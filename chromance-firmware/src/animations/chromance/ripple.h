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
    enum class RippleState
    {
        // Ripple is to be deleted and should not lit up
        Dead,
        // Ripple isn't drawn as it passes through a node to keep the speed consistent
        WithinNode,
        // Ripple is moving upwards
        TravelUp,
        // Ripple is moving downwards
        TravelDown
    };

    enum class RippleBehavior
    {
        // Stop at next node
        CouchPotato,
        // Only go straight
        Lazy,
        // Go straight if possible
        Weak,
        Feisty,
        Angry,
        AlwaysRight,
        AlwaysLeft,
        Exploding
    };

    class Ripple
    {
        public:

            Ripple();

            void Start(int node, int direction, CRGB color, float speed, unsigned long lifespan, RippleBehavior behavior);
            void Advance(CRGB* leds);
            void Claim(uint8_t animationId);
            RippleState GetState();
            uint8_t GetAnimationId();
        
        private:

            void Render(CRGB* leds, unsigned long age);

            RippleState state;
            CRGB color;
            // The node if within a node otherwise the segment if travelling
            int node;
            // The direction if within a node otherwise the LED postition from the bottom if travelling
            int direction;
            // Each loop the ripples move this many LED's
            float speed;
            // The ripple stops after this many milliseconds
            unsigned long lifespan;
            RippleBehavior behavior;
            bool justStarted;
            // The ripple will move When pressure reaches 1
            float pressure;
            // Used to track age of ripple
            unsigned long birthday;
            // The ID of the animation that has claimed this ripple
            uint8_t animationId;
        };
}

#endif
