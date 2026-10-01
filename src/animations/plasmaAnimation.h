#ifndef PLASMA_ANIMATION_H_
#define PLASMA_ANIMATION_H_

#include "animation.h"
#include "ledMap.h"

namespace Chromance
{
    class PlasmaAnimation : public Animation
    {
        public:

            PlasmaAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger);

            void Loop();

        private:

            LEDMap* ledMap;
            float z;
            float hue;
    };
}

#endif
