#include "centerPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

CenterPulseAnimation::CenterPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "centerPulse", ripplePool, logger)
{
}

void CenterPulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    float speed = random(500, 800) / 1000.0f;
    unsigned long lifespan = 5000U;
    
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
            StarburstNode,
            i,
            color,
            speed,
            lifespan,
            behavior
        );
    }    
}