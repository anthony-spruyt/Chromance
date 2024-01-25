#include "rippleAnimation.h"

using namespace Chromance;



RippleAnimation::RippleAnimation
(
    int32_t id,
    const char* name,
    RipplePool* ripplePool,
    Config* config,
    Logger* logger
) :
    Animation(id, name, config, logger)
{
    this->ripplePool = ripplePool;
}

void RippleAnimation::Loop()
{
    // Fade all dots to create trails
    nscale8(this->leds, NumberOfLEDs, this->config->GetRippleDecay(this->GetAnimationType()));

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

    if (now - this->lastPulse >= this->config->GetRipplePulsePeriod(this->GetAnimationType()))
    {
        this->Start();
        this->lastPulse = now;
    }
}

bool RippleAnimation::IsRippleAnimation()
{
    return true;
}

unsigned long RippleAnimation::GetLifespan()
{
    return this->config->GetRippleLifespan(this->GetAnimationType());
}