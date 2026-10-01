#ifndef FIRE_ANIMATION_H_
#define FIRE_ANIMATION_H_

#include "animation.h"
#include "ledMap.h"

namespace Chromance
{
    class FireAnimation : public Animation
    {
        public:

            FireAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger);

            void Loop();

        private:

            LEDMap* ledMap;
            float rise;
            float flicker;
    };
}

#endif
