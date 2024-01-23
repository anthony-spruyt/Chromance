#include "rainbowMarchAnimation.h"

using namespace Chromance;

RainbowMarchAnimation::RainbowMarchAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Rainbow March", config, logger)
{
}

void RainbowMarchAnimation::Loop()
{
    uint16_t speed = this->GetSpeed() * RainbowMarchAnimationSpeed;
    uint8_t hue = beat8(speed, UINT8_MAX);
  
    fill_rainbow(&this->leds[0], NumberOfLEDs, hue, RainbowMarchAnimationHueDelta);
}