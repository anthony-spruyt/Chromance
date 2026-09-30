#include "animationTemplate.h"

using namespace Chromance;

AnimationTemplate::AnimationTemplate(int32_t id, Config* config, Logger* logger) :
    Animation(id, "MyAnimationName", config, logger)
{
}

void AnimationTemplate::Loop()
{
}