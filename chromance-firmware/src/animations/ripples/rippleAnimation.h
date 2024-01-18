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

        protected:

            unsigned long GetLifespan();

            RipplePool* ripplePool;
            unsigned long lastPulse;
    };
}

#endif