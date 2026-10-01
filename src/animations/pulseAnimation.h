#ifndef PULSE_ANIMATION_H_
#define PULSE_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class PulseAnimation : public Animation
    {
        public:

            PulseAnimation(int32_t id, Config* config, Logger* logger);

            void Loop();

        private:

            void Reset() override;
            void NextColor();

            CRGB color;
    };
}

#endif
