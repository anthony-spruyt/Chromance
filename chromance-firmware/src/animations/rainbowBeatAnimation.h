#ifndef RAINBOWBEAT_ANIMATION_H_
#define RAINBOWBEAT_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class RainbowBeatAnimation : public Animation
    {
        public:

            RainbowBeatAnimation(uint8_t id);

            void Loop();
    };
}

#endif