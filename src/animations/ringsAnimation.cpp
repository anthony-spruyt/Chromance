#include "ringsAnimation.h"

using namespace Chromance;

RingsAnimation::RingsAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger) :
    Animation(id, "Rings", config, logger),
    ledMap(ledMap),
    phase(0.0f),
    hue(0.0f)
{
}

void RingsAnimation::Loop()
{
    float seconds = this->GetElapsedSeconds();
    uint8_t count = this->GetParameter(RINGS_COUNT);
    uint8_t distance;

    this->phase = fmodf(this->phase + this->GetParameter(RINGS_RING_SPEED) * seconds, 256.0f);
    this->hue = fmodf(this->hue + this->GetParameter(RINGS_COLOR_SPEED) * seconds, 256.0f);

    for (uint32_t i = 0; i < NumberOfLEDs; i++)
    {
        distance = this->ledMap->GetDistance(i);
        this->leds[i] = CHSV((uint8_t)this->hue - distance / 2U, UINT8_MAX, dim8_raw(sin8(distance * count - (uint8_t)this->phase)));
    }
}
