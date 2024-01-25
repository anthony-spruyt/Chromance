#ifndef RAINBOWBEAT_ANIMATION_H_
#define RAINBOWBEAT_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class RainbowBeatAnimation : public Animation
    {
        public:

            RainbowBeatAnimation(int32_t id, Config* config, Logger* logger);

            void Loop();
    };
}

#endif