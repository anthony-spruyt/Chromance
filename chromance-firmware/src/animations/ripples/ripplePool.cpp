#include "ripplePool.h"

using namespace Chromance;

RipplePool::RipplePool()
{
}

Ripple* RipplePool::Claim(int32_t animationId)
{
    for (int32_t i = 0; i < RipplePool::NumberOfRipples; i++)
    {
        if (this->ripples[i].GetState() == RIPPLE_STATE_DEAD)
        {
            this->ripples[i].Claim(animationId);

            return &this->ripples[i];
        }
    }
    
    return nullptr;
}

Ripple* RipplePool::Get(uint32_t index)
{
    if (index >= NumberOfRipples)
    {
        return nullptr;
    }

    return &this->ripples[index];
}