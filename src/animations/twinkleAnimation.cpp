#include "twinkleAnimation.h"

using namespace Chromance;

TwinkleAnimation::TwinkleAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Twinkle", config, logger),
    pendingTwinkles(0.0f)
{
}

void TwinkleAnimation::Loop()
{
    float seconds = this->GetElapsedSeconds();
    float fadeInTime = this->GetParameter(TWINKLE_FADE_IN_TIME) / 1000.0f;
    float fadeOutTime = this->GetParameter(TWINKLE_FADE_OUT_TIME) / 1000.0f;
    float lifetime = fadeInTime + fadeOutTime;
    float age;
    int32_t segment;
    uint8_t value;

    this->pendingTwinkles += this->GetParameter(TWINKLE_PER_SECOND) * seconds;

    while (this->pendingTwinkles >= 1.0f)
    {
        this->pendingTwinkles -= 1.0f;
        segment = random8(NumberOfSegments);

        // Restarting a lit segment would snap it dark, so that twinkle is skipped
        if (this->age[segment] >= lifetime)
        {
            this->age[segment] = 0.0f;
            this->hue[segment] = random8();
        }
    }

    for (segment = 0; segment < NumberOfSegments; segment++)
    {
        age = this->age[segment] = min(this->age[segment] + seconds, lifetime);
        value = age < fadeInTime ?
            (uint8_t)(age * UINT8_MAX / fadeInTime) :
            UINT8_MAX - (uint8_t)((age - fadeInTime) * UINT8_MAX / fadeOutTime);

        for (uint32_t step = 0; step < LEDsPerSegment; step++)
        {
            this->leds[SegmentLED(segment, step)] = CHSV(this->hue[segment], UINT8_MAX, dim8_raw(value));
        }
    }
}

void TwinkleAnimation::Reset()
{
    Animation::Reset();
    this->pendingTwinkles = 0.0f;

    for (int32_t segment = 0; segment < NumberOfSegments; segment++)
    {
        this->age[segment] = TwinkleAnimationParameters[TWINKLE_FADE_IN_TIME].max / 1000.0f + TwinkleAnimationParameters[TWINKLE_FADE_OUT_TIME].max / 1000.0f;
    }
}
