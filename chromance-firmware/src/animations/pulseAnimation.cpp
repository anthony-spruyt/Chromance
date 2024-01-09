#include "pulseAnimation.h"

using namespace Chromance;

PulseAnimation::PulseAnimation(uint8_t id) :
    Animation(id, "pulse")
{
}

void PulseAnimation::Loop()
{
    uint8_t scale = beatsin8(PulseAnimationSpeed, PulseAnimationMinBrightness, PulseAnimationMaxBrightness);
    fill_solid(this->leds, NumberOfLEDs, CRGB::DarkGreen);
    nscale8_video(this->leds, NumberOfLEDs, scale);
}