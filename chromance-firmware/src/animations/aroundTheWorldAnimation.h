#ifndef AROUNDTHEWORLD_ANIMATION_H_
#define AROUNDTHEWORLD_ANIMATION_H_

#include "ripples/rippleAnimation.h"

namespace Chromance
{
    class AroundTheWorldAnimation : public RippleAnimation
    {
        public:

            AroundTheWorldAnimation(int32_t id, RipplePool* ripplePool, Logger* logger);

            void Start();
    };
}

#endif