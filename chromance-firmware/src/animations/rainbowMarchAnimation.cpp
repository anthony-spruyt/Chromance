#include "rainbowMarchAnimation.h"

using namespace Chromance;

RainbowMarchAnimation::RainbowMarchAnimation(uint8_t id, Logger* logger) :
    Animation(id, "rainbowMarch", logger)
{
}

void RainbowMarchAnimation::Loop()
{
    uint8_t hue = beat8(RainbowAnimationSpeed, 255U);
  
    fill_rainbow(&this->leds[0], NumberOfLEDs, hue, RainbowAnimationHueDelta);
}