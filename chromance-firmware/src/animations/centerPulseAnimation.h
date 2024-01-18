#ifndef CENTERPULSE_ANIMATION_H_
#define CENTERPULSE_ANIMATION_H_

#include "ripples/rippleAnimation.h"

namespace Chromance
{
    class CenterPulseAnimation : public RippleAnimation
    {
        public:

            CenterPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:

            float speed;
            unsigned long lifespan;
    };
}

#endif