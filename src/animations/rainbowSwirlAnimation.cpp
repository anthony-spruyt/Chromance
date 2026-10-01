#include "rainbowSwirlAnimation.h"

using namespace Chromance;

RainbowSwirlAnimation::RainbowSwirlAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger) :
    Animation(id, "Rainbow Swirl", config, logger),
    ledMap(ledMap),
    hue(0.0f)
{
}

void RainbowSwirlAnimation::Loop()
{
    int32_t twist = this->GetParameter(RAINBOW_SWIRL_TWIST);

    this->hue = fmodf(this->hue + this->GetParameter(RAINBOW_SWIRL_SPIN_SPEED) * this->GetElapsedSeconds(), 256.0f);

    for (uint32_t i = 0; i < NumberOfLEDs; i++)
    {
        this->leds[i] = CHSV(this->ledMap->GetAngle(i) + this->ledMap->GetDistance(i) * twist / 256 - (uint8_t)this->hue, UINT8_MAX, UINT8_MAX);
    }
}
