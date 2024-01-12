#include "animationTemplate.h"

using namespace Chromance;

AnimationTemplate::AnimationTemplate(int32_t id, Logger* logger) :
    Animation(id, "MyAnimationName", logger)
{
}

void AnimationTemplate::Loop()
{
}