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
                Config* config,
                Logger* logger
            );

            void Loop() override;
            virtual void Start() = 0;
            bool IsRippleAnimation() override;

        protected:

            static constexpr unsigned long RippleStepMicros = 1000000UL / RippleStepsPerSecond;
            // Drops the backlog after a long stall instead of fast forwarding through it
            static constexpr uint32_t RippleMaxStepsPerFrame = 4U;

            void Reset() override;
            void Step();
            unsigned long GetLifespan();

            RipplePool* ripplePool;
            unsigned long lastPulse;
            unsigned long lastStep;
    };
}

#endif
