#ifndef CUBEPULSE_ANIMATION_H_
#define CUBEPULSE_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class CubePulseAnimation : public Animation
    {
        public:

            CubePulseAnimation(uint8_t id);

            void Loop();

        private:
    };
}

#endif