#include "ripplePool.h"

using namespace Chromance;

RipplePool::RipplePool()
{
}

Ripple* RipplePool::Claim(uint8_t animationId)
{
    for (int i = 0; i < RipplePool::NumberOfRipples; i++)
    {
        if (this->ripples[i].GetState() == RippleState::Dead)
        {
            this->ripples[i].Claim(animationId);

            return &this->ripples[i];
        }
    }
    
    return nullptr;
}

Ripple* RipplePool::Get(uint8_t index)
{
    if (index >= NumberOfRipples)
    {
        return nullptr;
    }

    return &this->ripples[index];
}