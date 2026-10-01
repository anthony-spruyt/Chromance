#ifndef TWINKLE_ANIMATION_H_
#define TWINKLE_ANIMATION_H_

#include "animation.h"
#include "ripples/map.h"

namespace Chromance
{
    class TwinkleAnimation : public Animation
    {
        public:

            TwinkleAnimation(int32_t id, Config* config, Logger* logger);

            void Loop();

        private:

            void Reset() override;

            float age[NumberOfSegments] = {};
            uint8_t hue[NumberOfSegments] = {};
            float pendingTwinkles;
    };
}

#endif
