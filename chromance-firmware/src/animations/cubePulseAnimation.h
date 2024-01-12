#ifndef CUBEPULSE_ANIMATION_H_
#define CUBEPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class CubePulseAnimation : public RippleAnimation
    {
        public:

            CubePulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:
            int32_t lastPulseNode;
    };
}

#endif