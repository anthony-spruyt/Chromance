#include "animationTemplate.h"

using namespace Chromance;

AnimationTemplate::AnimationTemplate(uint8_t id, Logger* logger) :
    Animation(id, "MyAnimationName", logger)
{
}

void AnimationTemplate::Loop()
{
}