#ifndef PULSE_ANIMATION_H_
#define PULSE_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class PulseAnimation : public Animation
    {
        public:

            PulseAnimation(uint8_t id, Logger* logger);

            void Loop();
            void Wake(bool fade) override;

        private:

            void NextColor();

            CRGB color;
    };
}

#endif