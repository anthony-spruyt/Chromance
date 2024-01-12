#ifndef RANDOMPULSE_ANIMATION_H_
#define RANDOMPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class RandomPulseAnimation : public RippleAnimation
    {
        public:

            RandomPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:
            int32_t lastPulseNode;
    };
}

#endif