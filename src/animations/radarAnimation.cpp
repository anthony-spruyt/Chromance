#include "radarAnimation.h"

using namespace Chromance;

RadarAnimation::RadarAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger) :
    Animation(id, "Radar", config, logger),
    ledMap(ledMap),
    sweep(0.0f),
    hue(0.0f)
{
}

void RadarAnimation::Loop()
{
    float seconds = this->GetElapsedSeconds();
    uint8_t trail = this->GetParameter(RADAR_TRAIL);
    uint8_t behind;
    uint8_t value;

    this->sweep = fmodf(this->sweep + this->GetParameter(RADAR_TURNS_PER_MINUTE) * 256.0f / 60.0f * seconds, 256.0f);
    this->hue = fmodf(this->hue + this->GetParameter(RADAR_COLOR_SPEED) * seconds, 256.0f);

    for (uint32_t i = 0; i < NumberOfLEDs; i++)
    {
        behind = (uint8_t)this->sweep - this->ledMap->GetAngle(i);
        value = behind < trail ? UINT8_MAX - behind * UINT8_MAX / trail : 0U;
        this->leds[i] = CHSV((uint8_t)this->hue, UINT8_MAX, dim8_raw(value));
    }
}
