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
                unsigned long pulsePeriod = 2000UL,
                uint8_t decay = 247U
            );

            void Loop() override;
            virtual void Start() = 0;

        protected:

            RipplePool* ripplePool;
            unsigned long pulsePeriod;
            uint8_t decay;
            unsigned long lastPulse;
    };
}

#endif