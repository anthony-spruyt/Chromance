#ifndef CENTERPULSE_ANIMATION_H_
#define CENTERPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class CenterPulseAnimation : public RippleAnimation
    {
        public:

            CenterPulseAnimation(uint8_t id, RipplePool* ripplePool, Logger* logger);

            void Start();
    };
}

#endif