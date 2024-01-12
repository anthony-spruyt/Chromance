#ifndef ANIMATION_TEMPLATE_H_
#define ANIMATION_TEMPLATE_H_

#include "animation.h"

namespace Chromance
{
    class AnimationTemplate : public Animation
    {
        public:

            AnimationTemplate(int32_t id, Logger* logger);

            void Loop();
    };
}

#endif