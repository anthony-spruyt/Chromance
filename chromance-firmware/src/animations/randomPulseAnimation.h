#ifndef RANDOMPULSE_ANIMATION_H_
#define RANDOMPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class RandomPulseAnimation : public RippleAnimation
    {
        public:

            RandomPulseAnimation(uint8_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:
            uint8_t lastPulseNode;
    };
}

#endif