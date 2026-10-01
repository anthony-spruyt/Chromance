#ifndef RAINBOWSWIRL_ANIMATION_H_
#define RAINBOWSWIRL_ANIMATION_H_

#include "animation.h"
#include "ledMap.h"

namespace Chromance
{
    class RainbowSwirlAnimation : public Animation
    {
        public:

            RainbowSwirlAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger);

            void Loop();

        private:

            LEDMap* ledMap;
            float hue;
    };
}

#endif
