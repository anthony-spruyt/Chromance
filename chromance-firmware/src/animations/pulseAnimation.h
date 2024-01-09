#ifndef PULSE_ANIMATION_H_
#define PULSE_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class PulseAnimation : public Animation
    {
        public:

            PulseAnimation(uint8_t id);

            void Loop();

        private:
    };
}

#endif