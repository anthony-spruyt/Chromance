#include "rippleAnimation.h"

using namespace Chromance;



RippleAnimation::RippleAnimation
(
    int32_t id,
    const char* name,
    RipplePool* ripplePool,
    Logger* logger,
    unsigned long pulsePeriod
) :
    Animation(id, name, logger)
{
    this->ripplePool = ripplePool;
    this->pulsePeriod = pulsePeriod;
}

void RippleAnimation::Loop()
{
    // Fade all dots to create trails
    nscale8(this->leds, NumberOfLEDs, RippleAnimation::Decay);

    // Advance this animations claimed ripples
    Ripple* ripple;

    for (int32_t i = 0; i < RipplePool::NumberOfRipples; i++)
    {
        ripple = ripplePool->Get(i);
        
        if (ripple->GetAnimationId() == this->id)
        {
            ripple->Advance(this->leds);
        }
    }

    // Determine if we need to fire off a new ripple
    unsigned long now = millis();

    if (now - this->lastPulse >= this->pulsePeriod)
    {
        this->Start();
        this->lastPulse = now;
    }
}