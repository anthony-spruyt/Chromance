#include "stripTestAnimation.h"

using namespace Chromance;

StripTestAnimation::StripTestAnimation(uint8_t id) :
    Animation(id, "stripTest"),
    counter(0),
    fillTest1(false),
    fillTest2(false),
    fillTest3(false),
    shouldWaitForFillTestNextFrame(false),
    shouldWaitForPixelTestNextFrame(false),
    lastFrame(0),
    nextPixelColorTest(0)
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
        fill_solid(this->leds, NumberOfLEDs, CRGB(255, 0, 255));
        this->fillTest1 = true;
        this->shouldWaitForFillTestNextFrame = true;
    }
    else if (!this->fillTest2)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB(0, 255, 255));
        this->fillTest2 = true;
        this->shouldWaitForFillTestNextFrame = true;
    }
    else if (!this->fillTest2)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB(255, 255, 0));
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
            case 0:
                leds[this->counter] = CRGB::Red;
                this->nextPixelColorTest++;
                this->shouldWaitForPixelTestNextFrame = true;
                break;
            case 1:
                leds[this->counter] = CRGB::Green;
                this->nextPixelColorTest++;
                this->shouldWaitForPixelTestNextFrame = true;
                break;
            case 2:
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