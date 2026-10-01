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
    Animation(id, name, config, logger),
    lastPulse(0UL)
{
    this->ripplePool = ripplePool;
}

void RippleAnimation::Loop()
{
    // Fade all dots to create trails
    nscale8(this->leds, NumberOfLEDs, this->config->GetRippleDecay(this->GetAnimationType()));

    Ripple* ripple;

    for (int32_t i = 0; i < RipplePool::NumberOfRipples; i++)
    {
        ripple = ripplePool->Get(i);

        if (ripple->GetAnimationId() == this->id)
        {
            ripple->Advance(this->leds);
        }
    }

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

void RippleAnimation::Reset()
{
    Animation::Reset();
    this->ripplePool->Release(this->id);
    this->lastPulse = 0UL;
}

unsigned long RippleAnimation::GetLifespan()
{
    return this->config->GetRippleLifespan(this->GetAnimationType());
}
