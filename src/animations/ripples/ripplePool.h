#ifndef RIPPLE_POOL_H_
#define RIPPLE_POOL_H_

#include "ripple.h"

namespace Chromance
{
    class RipplePool
    {
        public:

            RipplePool();

            /**
             * Claim a ripple from the pool
             * @param animationId The ID of the animation that is requesting to claim a ripple from the pool
             * @return A ripple that was successfully claimed by the caller
             */
            Ripple* Claim(int32_t animationId);
            /**
             * Get a ripple by index
             * @param index The index of the ripple
             * @return A ripple
             */
            Ripple* Get(uint32_t index);
            /**
             * Kill every ripple claimed by an animation so they return to the pool
             * @param animationId The ID of the animation releasing its ripples
             */
            void Release(int32_t animationId);

            static constexpr uint32_t NumberOfRipples = 30U;

        private:

            Ripple ripples[NumberOfRipples] = {
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple(),
                Ripple()
            };
    };
}

#endif
