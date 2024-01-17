#include "aroundTheWorldAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

AroundTheWorldAnimation::AroundTheWorldAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Around the World", ripplePool, logger)
{
    this->pulsePeriod = 3000UL;
}

void AroundTheWorldAnimation::Start()
{
    uint32_t baseColor = random(0xFFFF);
    float speed = random(100) / 100.0f * 6.0f + 2.0f;
    unsigned long lifespan = random(100) / 100.0f * 15000U + 5000U;
    
    RippleBehavior behavior = RIPPLE_BEHAVIOR_ALWAYS_LEFT;
    Ripple* ripple;
    

    ripple = this->ripplePool->Claim(this->id);

    if (ripple == nullptr)
    {
        this->logger->Warn("The ripple pool is empty");
    }

    ripple->Start
    (
        24,
        1,
        CHSV(baseColor, 255U, 255U),
        speed,
        lifespan,
        behavior
    );  
}