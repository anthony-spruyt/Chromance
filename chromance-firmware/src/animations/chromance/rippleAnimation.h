#ifndef RIPPLE_ANIMATION_H_
#define RIPPLE_ANIMATION_H_

#include "../animation.h"
#include "ripplePool.h"

namespace Chromance
{
    class RippleAnimation : public Animation
    {
        public:

            RippleAnimation(uint8_t id, const char* name, RipplePool* ripplePool, Logger* logger);

            void Loop() override;
            virtual void Start() = 0;

        protected:

            static const uint8_t Decay = 240;
            static const unsigned long PulsePeriod = 2000U;

            RipplePool* ripplePool;
            unsigned long lastPulse;
    };
}

#endif