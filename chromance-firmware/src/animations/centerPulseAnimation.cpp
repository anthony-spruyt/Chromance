#include "centerPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

CenterPulseAnimation::CenterPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Center Pulse", ripplePool, logger, 2800UL, 240U),
    speed(1.0f),
    lifespan(3000UL)
{
}

void CenterPulseAnimation::Start()
{
    CHSV color = CHSV(random8(), 255U, 255U);
    
    this->speed = random(100) / 100.0f * 0.3f + 1.0f;
    
    RippleBehavior behavior = RIPPLE_BEHAVIOR_FEISTY;
    Ripple* ripple;

    for (int32_t i = 0; i < MaxPathsPerNode; i++)
    {
        ripple = this->ripplePool->Claim(this->id);

        if (ripple == nullptr)
        {
            this->logger->Warn("The ripple pool is empty");
            
            break;
        }

        ripple->Start
        (
            CenterNode,
            i,
            color,
            this->speed,
            this->lifespan,
            behavior
        );
    }    
}