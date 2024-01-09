#include "rainbowBeatAnimation.h"

using namespace Chromance;

RainbowBeatAnimation::RainbowBeatAnimation(uint8_t id) :
    Animation(id, "rainbowBeat")
{
}

void RainbowBeatAnimation::Loop()
{
    uint8_t hue = beatsin8(RainbowAnimationSpeed, 0U, 255U);
  
    fill_rainbow(&this->leds[0], NumberOfLEDs, hue, RainbowAnimationHueDelta);
}