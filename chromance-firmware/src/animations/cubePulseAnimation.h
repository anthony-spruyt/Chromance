#ifndef CUBEPULSE_ANIMATION_H_
#define CUBEPULSE_ANIMATION_H_

#include "chromance/rippleAnimation.h"

namespace Chromance
{
    class CubePulseAnimation : public RippleAnimation
    {
        public:

            CubePulseAnimation(uint8_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:
            uint8_t lastPulseNode;
    };
}

#endif