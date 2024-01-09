#include "cubePulseAnimation.h"

using namespace Chromance;

CubePulseAnimation::CubePulseAnimation(uint8_t id) :
    Animation(id, "cubePulse")
{
}

void CubePulseAnimation::Loop()
{
    // TODO
    //https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino
    //uint8_t hue = 0U;

    // a colored dot sweeping back and forth, with fading trails
    //fadeToBlackBy(leds, NumberOfLEDs, 20);
    //uint16_t pos = beatsin16(13, 0, NumberOfLEDs - 1);
    //this->leds[pos] += CHSV(0, 255, 192);

    // random colored speckles that blink in and fade smoothly
    //fadeToBlackBy(this->leds, NumberOfLEDs, 10);
    //uint16_t pos = random16(NumberOfLEDs);
    //this->leds[pos] += CHSV(hue + random8(64), 200, 255);

    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    //uint8_t BeatsPerMinute = 62;
    //CRGBPalette16 palette = PartyColors_p;
    //uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    //for (int i = 0; i < NumberOfLEDs; i++)
    //{
    //    this->leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
    //}

    // eight colored dots, weaving in and out of sync with each other
    //fadeToBlackBy(this->leds, NumberOfLEDs, 20);
    //for (int i = 0; i < 8; i++)
    //{
    //    this->leds[beatsin16(i + 7, 0, NumberOfLEDs - 1)] |= CHSV(hue, 200, 255);
    //    hue += 32;
    //}
}