#include "plasmaAnimation.h"

using namespace Chromance;

PlasmaAnimation::PlasmaAnimation(int32_t id, LEDMap* ledMap, Config* config, Logger* logger) :
    Animation(id, "Plasma", config, logger),
    ledMap(ledMap),
    z(0.0f),
    hue(0.0f)
{
}

void PlasmaAnimation::Loop()
{
    float seconds = this->GetElapsedSeconds();
    uint8_t detail = this->GetParameter(PLASMA_DETAIL);
    uint8_t noise;

    this->z = fmodf(this->z + this->GetParameter(PLASMA_MORPH_SPEED) * seconds, 65536.0f);
    this->hue = fmodf(this->hue + this->GetParameter(PLASMA_COLOR_SPEED) * seconds, 256.0f);

    for (uint32_t i = 0; i < NumberOfLEDs; i++)
    {
        noise = inoise8(this->ledMap->GetX(i) * detail, this->ledMap->GetY(i) * detail, (uint16_t)this->z);
        this->leds[i] = ColorFromPalette(PartyColors_p, noise + (uint8_t)this->hue);
    }
}
