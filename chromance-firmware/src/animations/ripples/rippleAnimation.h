#ifndef RIPPLE_ANIMATION_H_
#define RIPPLE_ANIMATION_H_

#include "../animation.h"
#include "ripplePool.h"

namespace Chromance
{
    class RippleAnimation : public Animation
    {
        public:

            RippleAnimation
            (
                int32_t id,
                const char* name,
                RipplePool* ripplePool,
                Logger* logger,
                unsigned long pulsePeriod = 2000UL
            );

            void Loop() override;
            virtual void Start() = 0;

        protected:

            static const uint8_t Decay = 240U;
            unsigned long pulsePeriod;

            RipplePool* ripplePool;
            unsigned long lastPulse;
    };
}

#endif