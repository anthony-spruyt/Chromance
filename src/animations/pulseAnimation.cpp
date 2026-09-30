#include "pulseAnimation.h"

using namespace Chromance;

PulseAnimation::PulseAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Pulse", config, logger),
    color(PulseAnimationColors[0])
{
}

void PulseAnimation::Loop()
{
    uint16_t speed = this->GetSpeed() * PulseAnimationSpeed;
    uint8_t scale = beatsin8(speed, PulseAnimationMinBrightness, PulseAnimationMaxBrightness);
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