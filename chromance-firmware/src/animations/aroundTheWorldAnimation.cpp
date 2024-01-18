#include "aroundTheWorldAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

AroundTheWorldAnimation::AroundTheWorldAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Around the World", ripplePool, logger, 3000UL),
    speed(1.0f),
    lifespan(5000UL)
{
    this->pulsePeriod = 3000UL;
}

void AroundTheWorldAnimation::Start()
{
    this->speed = random(100) / 100.0f * 6.0f + 2.0f;
    this->lifespan = random(100) / 100.0f * 15000U + 5000U;
    
    RippleBehavior behavior = RIPPLE_BEHAVIOR_ALWAYS_LEFT;
    Ripple* ripple;
    

    ripple = this->ripplePool->Claim(this->id);

    if (ripple == nullptr)
    {
        this->logger->Warn("The ripple pool is empty");
    }

    ripple->Start
    (
        BottomNode,
        1,
        CHSV(random8(), 255U, 255U),
        this->speed,
        this->lifespan,
        behavior
    );  
}