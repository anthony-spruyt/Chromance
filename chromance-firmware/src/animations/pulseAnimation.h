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
            void Wake(bool fade) override;

        private:

            void NextColor();

            CRGB color;
    };
}

#endif