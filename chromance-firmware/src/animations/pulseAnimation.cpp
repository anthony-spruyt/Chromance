#include "pulseAnimation.h"

using namespace Chromance;

PulseAnimation::PulseAnimation(uint8_t id, Logger* logger) :
    Animation(id, "pulse", logger),
    color(PulseAnimationColors[0])
{
}

void PulseAnimation::Loop()
{
    uint8_t scale = beatsin8(PulseAnimationSpeed, PulseAnimationMinBrightness, PulseAnimationMaxBrightness);
    fill_solid(this->leds, NumberOfLEDs, this->color);
    nscale8_video(this->leds, NumberOfLEDs, scale);
}

void PulseAnimation::Wake(bool fade)
{
    this->NextColor();
    Animation::Wake(fade);
}

void PulseAnimation::NextColor()
{
    uint8_t index = random8(PulseAnimationNumberOfColors);
    this->color = CRGB(PulseAnimationColors[index]);
}