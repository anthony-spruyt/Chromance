#include "rainbowBeatAnimation.h"

using namespace Chromance;

RainbowBeatAnimation::RainbowBeatAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Rainbow Beat", config, logger)
{
}

void RainbowBeatAnimation::Loop()
{
    uint16_t speed = this->GetSpeed() * RainbowBeatAnimationSpeed;
    uint8_t hue = beatsin8(speed, 0U, UINT8_MAX);
  
    fill_rainbow(&this->leds[0], NumberOfLEDs, hue, RainbowBeatAnimationHueDelta);
}