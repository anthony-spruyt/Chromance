#include "fireAnimation.h"

using namespace Chromance;

FireAnimation::FireAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger) :
    Animation(id, "Fire", config, logger),
    ledMap(ledMap),
    rise(0.0f),
    flicker(0.0f)
{
}

void FireAnimation::Loop()
{
    float seconds = this->GetElapsedSeconds();
    uint8_t detail = this->GetParameter(FIRE_DETAIL);
    uint8_t cooling = this->GetParameter(FIRE_COOLING);
    uint8_t y;
    uint8_t heat;

    // Sampling further down the noise each frame moves the flames up the wall
    this->rise = fmodf(this->rise + this->GetParameter(FIRE_RISE_SPEED) * seconds, 65536.0f);
    this->flicker = fmodf(this->flicker + this->GetParameter(FIRE_FLICKER_SPEED) * seconds, 65536.0f);

    for (uint32_t i = 0; i < NumberOfLEDs; i++)
    {
        y = this->ledMap->GetY(i);
        heat = inoise8(this->ledMap->GetX(i) * detail, y * detail + (uint16_t)this->rise, (uint16_t)this->flicker);
        heat = qsub8(heat, scale8(UINT8_MAX - y, cooling));
        this->leds[i] = ColorFromPalette(HeatColors_p, scale8(heat, FireAnimationMaxPaletteIndex));
    }
}
