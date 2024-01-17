#ifndef CUBEPULSE_ANIMATION_H_
#define CUBEPULSE_ANIMATION_H_

#include "ripples/rippleAnimation.h"

namespace Chromance
{
    class CubePulseAnimation : public RippleAnimation
    {
        public:

            CubePulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();

        private:

            static const int32_t NumberOfCubeNodes = 8;

            int32_t cubeNodes[NumberOfCubeNodes] = {7, 8, 9, 11, 12, 17, 18, 20};
            float speed;
            unsigned long lifespan;
            int32_t lastPulseNode;
    };
}

#endif