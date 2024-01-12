#ifndef STARBURSTPULSE_ANIMATION_H_
#define STARBURSTPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class StarBurstPulseAnimation : public RippleAnimation
    {
        public:

            StarBurstPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();
    };
}

#endif