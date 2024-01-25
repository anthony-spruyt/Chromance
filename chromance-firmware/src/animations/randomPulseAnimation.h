#ifndef RANDOMPULSE_ANIMATION_H_
#define RANDOMPULSE_ANIMATION_H_

#include "ripples/rippleAnimation.h"

namespace Chromance
{
    class RandomPulseAnimation : public RippleAnimation
    {
        public:

            RandomPulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger);

            void Start();

        private:

            static const int32_t NumberOfRandomPulseNodes = 9;
            
            int32_t randomPulseNodes[NumberOfRandomPulseNodes] = {7, 8, 9, 11, 12, 15, 17, 18, 20};
            int32_t lastPulseNode;
    };
}

#endif