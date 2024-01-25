#ifndef STARBURSTPULSE_ANIMATION_H_
#define STARBURSTPULSE_ANIMATION_H_

#include "ripples/rippleAnimation.h"

namespace Chromance
{
    class StarBurstPulseAnimation : public RippleAnimation
    {
        public:

            StarBurstPulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger);

            void Start();
    };
}

#endif