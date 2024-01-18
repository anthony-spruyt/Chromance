#include "rainbowBeatAnimation.h"

using namespace Chromance;

RainbowBeatAnimation::RainbowBeatAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Rainbow Beat", config, logger)
{
}

void RainbowBeatAnimation::Loop()
{
    uint8_t hue = beatsin8(RainbowAnimationSpeed, 0U, 255U);
  
    fill_rainbow(&this->leds[0], NumberOfLEDs, hue, RainbowAnimationHueDelta);
}