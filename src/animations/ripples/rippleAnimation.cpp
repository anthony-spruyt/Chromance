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
    lastPulse(0UL),
    lastStep(0UL)
{
    this->ripplePool = ripplePool;
}

void RippleAnimation::Loop()
{
    unsigned long nowMicros = micros();
    unsigned long stepMicros = 1000000UL / this->config->GetRippleStepsPerSecond();
    uint32_t steps = 0U;

    // Fixed rate steps keep ripple speed and trail length the same at any frame rate
    while (nowMicros - this->lastStep >= stepMicros)
    {
        if (steps == RippleMaxStepsPerFrame)
        {
            this->lastStep = nowMicros;

            break;
        }

        this->Step();
        this->lastStep += stepMicros;
        steps++;
    }

    unsigned long now = millis();

    if (now - this->lastPulse >= this->config->GetRipplePulsePeriod(this->GetAnimationType()))
    {
        this->Start();
        this->lastPulse = now;
    }
}

void RippleAnimation::Step()
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
    this->lastStep = micros();
}

unsigned long RippleAnimation::GetLifespan()
{
    return this->config->GetRippleLifespan(this->GetAnimationType());
}
