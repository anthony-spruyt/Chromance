#include "centerPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

CenterPulseAnimation::CenterPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Center Pulse", ripplePool, logger)
{
}

void CenterPulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    float speed = random(100) / 100.0f * 0.8f + 2.0f;
    unsigned long lifespan = 3000UL;
    
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
            StarBurstNode,
            i,
            color,
            speed,
            lifespan,
            behavior
        );
    }    
}