#ifndef RINGS_ANIMATION_H_
#define RINGS_ANIMATION_H_

#include "animation.h"
#include "ledMap.h"

namespace Chromance
{
    class RingsAnimation : public Animation
    {
        public:

            RingsAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger);

            void Loop();

        private:

            LEDMap* ledMap;
            float phase;
            float hue;
    };
}

#endif
