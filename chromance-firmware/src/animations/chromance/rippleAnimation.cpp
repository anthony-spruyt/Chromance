#include "rippleAnimation.h"

using namespace Chromance;



RippleAnimation::RippleAnimation(uint8_t id, const char* name, RipplePool* ripplePool, Logger* logger) :
    Animation(id, name, logger)
{
    this->ripplePool = ripplePool;
}

void RippleAnimation::Loop()
{
    // Fade all dots to create trails
    nscale8(this->leds, NumberOfLEDs, RippleAnimation::Decay);

    // Advance this animations claimed ripples
    Ripple* ripple;

    for (int i = 0; i < RipplePool::NumberOfRipples; i++)
    {
        ripple = ripplePool->Get(i);
        
        if (ripple->GetAnimationId() == this->id)
        {
            ripple->Advance(this->leds);
        }
    }

    // Determine if we need to fire off a new ripple
    unsigned long now = millis();

    if (now - this->lastPulse >= PulsePeriod)
    {
        this->Start();
        this->lastPulse = now;
    }
}