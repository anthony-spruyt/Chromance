#ifndef RAINBOWMARCH_ANIMATION_H_
#define RAINBOWMARCH_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class RainbowMarchAnimation : public Animation
    {
        public:

            RainbowMarchAnimation(int32_t id, Logger* logger);

            void Loop();
    };
}

#endif