#include "stripTestAnimation.h"

using namespace Chromance;

StripTestAnimation::StripTestAnimation(int32_t id, Config* config, Logger* logger) :
    Animation(id, "Strip Test", config, logger),
    counter(0U),
    fillTest1(false),
    fillTest2(false),
    fillTest3(false),
    shouldWaitForFillTestNextFrame(false),
    shouldWaitForPixelTestNextFrame(false),
    lastFrame(0UL),
    nextPixelColorTest(0U)
{
}

void StripTestAnimation::Loop()
{
    if (this->shouldWaitForFillTestNextFrame)
    {
        this->shouldWaitForFillTestNextFrame = false;
        delay(StripTestAnimation::FillTestMillis);
    }
    else if (this->shouldWaitForPixelTestNextFrame)
    {
        this->shouldWaitForPixelTestNextFrame = false;
        delay(StripTestAnimation::PixelTestMillis);
    }

    unsigned long now = millis();

    if (!this->fillTest1)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB(UINT8_MAX, 0U, UINT8_MAX));
        this->fillTest1 = true;
        this->shouldWaitForFillTestNextFrame = true;
    }
    else if (!this->fillTest2)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB(0U, UINT8_MAX, UINT8_MAX));
        this->fillTest2 = true;
        this->shouldWaitForFillTestNextFrame = true;
    }
    else if (!this->fillTest2)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB(UINT8_MAX, UINT8_MAX, 0U));
        this->fillTest3 = true;
        this->shouldWaitForFillTestNextFrame = true;
    }
    else
    {
        if (this->counter >= NumberOfLEDs)
        {
            this->counter = 0;
            this->fillTest1 = false;
            this->fillTest2 = false;
            this->fillTest3 = false;
            this->shouldWaitForFillTestNextFrame = false;
            this->shouldWaitForPixelTestNextFrame = false;

            return;
        }

        fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
        
        switch (this->nextPixelColorTest)
        {
            case 0U:
                leds[this->counter] = CRGB::Red;
                this->nextPixelColorTest++;
                this->shouldWaitForPixelTestNextFrame = true;
                break;
            case 1U:
                leds[this->counter] = CRGB::Green;
                this->nextPixelColorTest++;
                this->shouldWaitForPixelTestNextFrame = true;
                break;
            case 2U:
                leds[this->counter] = CRGB::Blue;
                this->nextPixelColorTest = 0;
                this->counter++;
                this->shouldWaitForPixelTestNextFrame = true;
                break;
            default:
                break;
        }
    }
}