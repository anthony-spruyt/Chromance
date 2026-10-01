#ifndef RADAR_ANIMATION_H_
#define RADAR_ANIMATION_H_

#include "animation.h"
#include "ledMap.h"

namespace Chromance
{
    class RadarAnimation : public Animation
    {
        public:

            RadarAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger);

            void Loop();

        private:

            LEDMap* ledMap;
            float sweep;
            float hue;
    };
}

#endif
